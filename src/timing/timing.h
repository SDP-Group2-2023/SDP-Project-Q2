#ifndef TIMING_H
#define TIMING_H
#include <chrono>
#include <vector>

enum class timing_flag { TIMING_NOW, TIMING_DEFER};

class timing {
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> begins;
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> ends;

    bool running;

public:
    timing();
    explicit timing(timing_flag flag);
    void start();
    void stop();
    void reset();
    std::chrono::milliseconds getDuration();
};

#endif