#pragma once

#include <cstdio>
#include <cstdarg>

class Logger {
    FILE * logfile;
public:
    Logger () : logfile (stderr) {}
    Logger (const char * filename) {
        logfile = fopen(filename, "w");
        if (!logfile) {
            logfile = stderr;
            fprintf(stderr, "Logger: fail open file %s, using stderr.\n", filename);
        }
    }
    ~Logger () {
        if (logfile != stderr) {
            fclose(logfile);
        }
    }
    void printf(const char * fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vfprintf(logfile, fmt, args);
        va_end(args);
        fflush(logfile);
    }
};
