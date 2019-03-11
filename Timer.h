#pragma once

#include <chrono>

class Timer {
public:
    using time_point = std::chrono::high_resolution_clock::time_point;
    static time_point now() {
        return std::chrono::high_resolution_clock::now();
    }
    Timer() {reset();}

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

#ifndef infofln
#define infofln(fmt, ...) fprintf(stderr, fmt"\n", __VA_ARGS__)
#endif infofln

struct scoped_timer_logger {
    const char * tag;
    Timer t;
    scoped_timer_logger(const char * tag_) {
        tag = tag_;
        t.reset();
    }
    ~scoped_timer_logger() {
        infofln("[timing] %s for %lg ms", tag, t.duration_in_seconds() * 1000);
    }
};

#define TIME_SCOPE(tag) scoped_timer_logger __sl(tag)