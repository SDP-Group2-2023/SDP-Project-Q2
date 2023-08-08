#include "timing.h"

using namespace std::chrono;

timing::timing() : running(true) {
    begins.push_back(high_resolution_clock::now());
}

timing::timing(int flag) {
    if (flag == TIMING_NOW) {
        running = true;
        begins.push_back(high_resolution_clock::now());
    } else if (flag == TIMING_DEFER)
        running = false;
}

void timing::start() {
    if (running)
        return;
    running = true;
    begins.push_back(high_resolution_clock::now());
}

void timing::stop() {
    if (!running)
        return;
    running = false;
    ends.push_back(high_resolution_clock::now());
}

void timing::reset() {
    running = false;
    begins.clear();
    ends.clear();
}

std::chrono::milliseconds timing::getDuration() {
    if (running || begins.size() == 0)
        return duration_cast<milliseconds>(high_resolution_clock::now() - high_resolution_clock::now());
    auto tot_duration = (ends[0] - begins[0]);

    for (int i = 1; i < begins.size(); i++)
        tot_duration = tot_duration + (ends[i] - begins[i]);

    return duration_cast<milliseconds>(tot_duration);
}
