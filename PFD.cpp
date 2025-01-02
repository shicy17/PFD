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
#include <cmath>  
#include "PFD.h"
#include <windows.h>

typedef std::vector<Event2> EventList;

void write_denoised_data_to_txt(const EventList& denoised_data, const std::string& file_path) {
    std::ofstream file(file_path, std::ios::app);
    for (const auto& event : denoised_data) {
        file << event.x << " " << event.y << " " << event.p << " "
            << event.timestamp << "\n";
    }
}


void processEvents(std::string path, int delta_t0, int delta_t, int var, int neibor, std::string event_txt, bool change_negative_polarity, bool score_seletct) {
    std::string output_txt = path + "denoised_" + std::to_string(delta_t) + "_" + std::to_string(neibor) + ".txt";
    std::ofstream file(output_txt, std::ios::app);

    std::vector<std::vector<double>> P_time(REY, std::vector<double>(REX, 0.0));
    std::vector<std::vector<double>> N_time(REY, std::vector<double>(REX, 0.0));
    std::vector<std::vector<int>> Count_img_pol(REY, std::vector<int>(REX, 0));
    std::vector<std::vector<int>> polarity_img(REY, std::vector<int>(REX, 0));
    std::vector<std::vector<int>> Count_img(REY, std::vector<int>(REX, 0));

    Event2 event{}, event2{};
    long double t = 0, t0 = 0, slice_begin_time = 0;
    int x = 0, y = 0, p = 0;
    std::ifstream event_file(event_txt, std::ifstream::in);
    event_file >> x >> y >> p >> t0;
    EventList data, denoised_data2;

    // Process programme
    while (event_file >> x >> y >> p >> t) {
        event.timestamp = t - t0;
        event.x = x;
        event.y = y;
        t = event.timestamp;

        if (change_negative_polarity) {
            event.p = p;
        }
        else {
            event.p = (p == 0) ? -1 : 1;
        }

        if (polarity_img[event.y][event.x] == 0) {
            polarity_img[event.y][event.x] = event.p;
        }
        else {
            int flag = polarity_img[event.y][event.x] * event.p;
            if (flag == -1) {
                Count_img_pol[event.y][event.x]++;
            }
        }

        polarity_img[event.y][event.x] = event.p;
        Count_img[event.y][event.x]++;

        //First stage
        int yuflag = 0;
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
            if (yuflag >= var) {
                data.push_back(event);
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
            if (yuflag >= var) {
                data.push_back(event);
            }
        }

        //Second stage
        if (event.timestamp - slice_begin_time >= delta_t) {
            slice_begin_time = t;

            for (int i = 0; i < data.size(); i++) {

                event2 = data[i];
                int x = data[i].x;
                int y = data[i].y;
                double current_polarity_changes = Count_img_pol[y][x];
                double neighbor_polarity_changes = 0;
                double event_number = 0;
                int neighbors_count = 0;
                double score = 0;
                double score1 = 0;

                if (Count_img[y][x] != 0) {
                    for (int ii = y - 1; ii <= y + 1; ++ii) {
                        for (int jj = x - 1; jj <= x + 1; ++jj) {
                            if ((ii != y || jj != x) && ii >= 0 && ii <= REY - 1 && jj >= 0 && jj <= REX - 1) {
                                event_number += Count_img[ii][jj];
                                neighbor_polarity_changes += Count_img_pol[ii][jj];
                                if (Count_img[ii][jj] != 0) {
                                    neighbors_count++;
                                }
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


                if (neighbors_count > neibor && score <= 1) {
                    denoised_data2.push_back(event2);
                }
            }

            for (auto& row : Count_img) {
                std::fill(row.begin(), row.end(), 0);
            }
            for (auto& row : Count_img_pol) {
                std::fill(row.begin(), row.end(), 0);
            }
            for (auto& row : polarity_img) {
                std::fill(row.begin(), row.end(), 0);
            }

            write_denoised_data_to_txt(denoised_data2, output_txt);
            denoised_data2.clear();
            data.clear();

        }
    }
    event_file.close();

}

int main() {
    processEvents(path, delta_t0, delta_t, var, neibor, event_txt, change_negative_polarity, score_seletct);
}
