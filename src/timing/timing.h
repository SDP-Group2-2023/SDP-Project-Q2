#ifndef TIMING_H
#define TIMING_H
#define TIMING_NOW   0
#define TIMING_DEFER 1

#include <chrono>
#include <vector>

class timing {
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> begins;
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> ends;

    bool running;

public:
    timing();
    explicit timing(int flag);
    void start();
    void stop();
    void reset();
    std::chrono::milliseconds getDuration();
};

#endif