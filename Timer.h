#pragma once

#include <chrono>

class Timer {
public:
    using time_point = std::chrono::high_resolution_clock::time_point;
    static time_point now() {
        return std::chrono::high_resolution_clock::now();
    }

    Timer() {
        reset();
    }

    double duration_in_seconds() const {
        time_point end = now();
        auto count = std::chrono::duration_cast<std::chrono::duration<double>>(
                end - starting_point_)
            .count();
        return count;
    }

    void reset() {
        starting_point_ = now();
    }

private:
    time_point starting_point_;
};
