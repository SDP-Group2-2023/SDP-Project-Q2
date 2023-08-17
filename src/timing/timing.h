#ifndef TIMING_H
#define TIMING_H
#define TIMING_NOW   0
#define TIMING_DEFER 1

#include <chrono>
#include <vector>

using namespace std::chrono;
class timing {
    std::vector<time_point<high_resolution_clock>> begins;
    std::vector<time_point<high_resolution_clock>> ends;

    bool running;

public:
    timing();
    timing(int flag);
    void start();
    void stop();
    void reset();
    std::chrono::milliseconds getDuration();
};

#endif