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


#include <string>
#include <vector>
#include <algorithm>

class TimerManager {
public:
    std::vector<std::string> name;
    std::vector<std::vector<double>> record;
    void recordTime(std::string n, double r) {
        int i = std::find(name.begin(), name.end(), n) - name.begin();
        record[i].push_back(r);
    }
    struct scoped_timer {
        scoped_timer(TimerManager & m, std::string n) : owner(m), name(n) {}
        scoped_timer(scoped_timer &&) = default;
        scoped_timer& operator=(scoped_timer &&) = default;
        scoped_timer(const scoped_timer &) = delete;
        scoped_timer& operator=(const scoped_timer &) = delete;
        ~scoped_timer() {
            owner.recordTime(name, t.duration_in_seconds());
        }
        TimerManager & owner;
        std::string name;
        Timer t;
    };
public:
    scoped_timer scope_timer(std::string n) {
        if (std::find(name.begin(), name.end(), n) == name.end()) {
            name.push_back(n);
            record.push_back({});
        }
        return scoped_timer(*this, n);
    }
    void print(FILE * fp) {
        for (int i = 0; i < name.size(); i ++) {
            fprintf(fp, "%s", name[i].c_str());
            for (auto t : record[i]) {
                fprintf(fp, " %lg", t);
            }
            fprintf(fp, "\n");
        }
    }
};
