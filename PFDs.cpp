/*
 * Copyright (C) 2024 Beihang University, Neuromorphic Vision Perception and Computing Group
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright ? Beihang University, Neuromorphic Vision Perception and Computing Group.
 * License: This code is licensed under the GNU General Public License v3.0.
 * You can redistribute it and/or modify it under the terms of the GPL-3.0 License.
 */

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ctime>
#include <cmath>  
#include "PFD.h"

typedef std::vector<Event2> EventList;
typedef std::array<std::array<EventList, REX>, REY> PixelGrid;


void write_denoised_data_to_txt(const EventList& denoised_data, const std::string& file_path) {
    std::ofstream file(file_path);
    for (const auto& event : denoised_data) {
        file << event.timestamp << " " << event.x << " "
            << event.y << " " << event.p << "\n";
    }
}

void processEvents(std::string path, int delta_t0, int delta_t, int var, int neibor, std::string event_txt, bool change_negative_polarity, bool score_seletct) {//Process duration
    std::string output_txt = path + "_denoised_" + std::to_string(delta_t) + "_" + std::to_string(neibor) + ".txt";

    std::vector<std::vector<CircularFIFO<int, fifoSize>>> matrix(REY, std::vector<CircularFIFO<int, fifoSize>>(REX));


    //Define the vector for processing
    double Count_img_pol[REY][REX] = {};
    double latest_timestamp[REY][REX] = { 0 };
    int polarity_img[REY][REX] = {};
    int Count_img[REY][REX] = {};
    std::vector<std::vector<double>> P_time(REY, std::vector<double>(REX, 0.0));
    std::vector<std::vector<double>> N_time(REY, std::vector<double>(REX, 0.0));

    Event2 event;
    int x = 0, y = 0;
    int p = 0;
    double long t = 0;
    std::ifstream event_file(event_txt, std::ifstream::in);
    EventList data;
    //Process programme
    while (event_file >> x >> y >> p >> t) {

        event.timestamp = t;//Note the unit of time, the default value is microseconds. 
        event.x = x;
        event.y = y;
        t = event.timestamp;


        if (change_negative_polarity) {
            event.p = p;
        }
        else {
            if (p == 0) {
                event.p = -1;
            }
            else {
                event.p = 1;
            }
        }


        if (polarity_img[event.y][event.x] == 0) {
            polarity_img[event.y][event.x] = event.p;
        }
        else
        {
            int flag = polarity_img[event.y][event.x] * event.p;
            if (flag == -1) {
                matrix[event.y][event.x].push(t);
            }
        }

        //First stage
        int yuflag = 0;
        bool first_judgement = 0;
        if (event.p == 1) {
            P_time[event.y][event.x] = event.timestamp;
            for (int ii = -1; ii <= 1; ii++) {
                for (int jj = -1; jj <= 1; jj++) {
                    if (ii != 0 || jj != 0) {
                        if (y + ii >= 0 && y + ii <= REY - 1 && x + jj >= 0 && x + jj <= REX - 1) {
                            if (event.timestamp - P_time[y + ii][x + jj] < delta_t0 && P_time[y + ii][x + jj] != 0) {
                                yuflag++;
                            }
                        }
                    }
                }
            }
        }
        if (event.p == -1) {
            N_time[event.y][event.x] = event.timestamp;
            for (int ii = -1; ii <= 1; ++ii) {
                for (int jj = -1; jj <= 1; ++jj) {
                    if (ii != 0 || jj != 0) {
                        if (y + ii >= 0 && y + ii <= REY - 1 && x + jj >= 0 && x + jj <= REX - 1) {
                            if (event.timestamp - N_time[y + ii][x + jj] < delta_t0 && N_time[y + ii][x + jj] != 0) {
                                yuflag++;
                            }
                        }
                    }
                }
            }
        }
        if (yuflag >= var) {
            first_judgement = 1;
        }


        //Second stage, event-by-event processing version
        latest_timestamp[event.y][event.x] = t;
        int neighbors_count = 0;
        int neighbor_polarity_changes = 0;
        int score = 0;
        int current_polarity_changes = 0;
        int polarity_change_pixel_bool = 0;
        int polarity_change_pixel = 0;
        if (event.y != REY - 1 && event.y != 0 && event.x != 0 && event.x != REX - 1) {
            for (int i = 0; i < matrix[event.y][event.x].size(); i++) {
                if (t - matrix[event.y][event.x][i] <= delta_t)
                {
                    current_polarity_changes++;
                }
            }
            for (int ii = y - 1; ii <= y + 1; ++ii) {
                for (int jj = x - 1; jj <= x + 1; ++jj) {
                    if (ii != y || jj != x) { // ÅÅ³ýµ±Ç°ÏñËØ

                        if (t - latest_timestamp[ii][jj] <= delta_t) {
                            neighbors_count++;
                        }

                        if (matrix[ii][jj].size() != 0) {
                            for (int i = 0; i < matrix[ii][jj].size(); i++) {
                                if (t - matrix[ii][jj][i] <= delta_t)
                                {
                                    neighbor_polarity_changes++;
                                    polarity_change_pixel_bool = 1;
                                }
                            }
                            if (polarity_change_pixel_bool == 1) {
                                polarity_change_pixel++;
                            }
                        }

                        polarity_change_pixel_bool = 0;

                    }

                }

            }
        }

        if (score_seletct == 1)
        {
            score = std::abs(current_polarity_changes - neighbor_polarity_changes / static_cast<double>(neighbors_count));
        }
        else {
            score = std::abs(neighbor_polarity_changes) / static_cast<double>(neighbors_count);
        }


        if (neighbors_count > neibor && score <= 1 && first_judgement == 1) {
            data.push_back(event);
        }

    }
    event_file.close();
    write_denoised_data_to_txt(data, output_txt);

}

int main()
{
    processEvents(path, delta_t0, delta_t, var, neibor, event_txt, change_negative_polarity, score_seletct);
}
