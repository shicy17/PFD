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

#ifndef PFD
#define PFD

const int REX = 1280;//Resolution of event camera (width)
const int REY = 720;//Resolution of event camera (height)
const bool change_negative_polarity = 0;//Change the represenatation pattern of polarity
const size_t fifoSize = 5;//FIFO size for second stage process
const std::string event_txt = "******";//input filepath
const std::string path = "******";//output path
const int delta_t0 = 25000;//time window for first stage (us)
const int delta_t = 25000;//time window for second stage (us)
const int var = 1;//parameter for first stage (us)
const int neibor = 3;//parameter for secon stage (us)
const bool score_seletct = 1;//1 for PFD-A, 0 for PFD-B

struct Event
{
    int x;
    int y;
    int p;
    long double timestamp;
    std::string label;
};

struct Event2
{
    int x;
    int y;
    int p;
    long double timestamp;
};


template<typename T, size_t Size>
class CircularFIFO {
public:
    void push(const T& value) {
        buffer[tail] = value;
        tail = (tail + 1) % Size;
        if (full) {
            head = (head + 1) % Size;
        }
        full = tail == head;
    }

    T& front() {
        if (empty()) {
            throw std::runtime_error("Attempted to access empty queue");
        }
        return buffer[head];
    }

    void pop() {
        if (empty()) {
            return;
        }
        full = false;
        head = (head + 1) % Size;
    }

    bool empty() const {
        return (!full && (head == tail));
    }

    size_t size() const {
        if (full) {
            return Size;
        }
        if (tail >= head) {
            return tail - head;
        }
        return Size + tail - head;
    }

    T& operator[](size_t index) {
        if (index >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return buffer[(head + index) % Size];
    }

private:
    std::vector<T> buffer = std::vector<T>(Size);
    size_t head = 0;
    size_t tail = 0;
    bool full = false;
};
#endif
