#include "logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <chrono>

static int logging_level = LOGGING_INFO;
static FILE * logging_output_fp = NULL;

const char * get_level_str(int level) {
    switch(level) {
        case LOGGING_DEBUG: return "DEBUG";
        case LOGGING_INFO: return "INFO";
        case LOGGING_WARNING: return "WARNING";
        case LOGGING_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

static void set_output_from_env() {
    const char * filename = getenv("LOGGING_OUTPUT");
    if (filename) {
        infofln("logging to file \"%s\"", filename);
        logging_output_fp = fopen(filename, "w");
        if (!logging_output_fp) errorfln("cannot open \"%s\"", filename);
    } else {
        infofln("output file not set (using env LOGGING_OUTPUT)");
    }
}

static void set_level_from_env() {
    char * env_level = getenv("LOGGING_LEVEL");
    if (env_level) {
        int l = LOGGING_INFO;
        if (sscanf(env_level, "%d", &l)) { // digit input
            logging_setlevel(l);
        } else {
            for (int l = 0; l < LOGGING_LEVEL_COUNT; l++)
                if (strcmp(get_level_str(l), env_level) == 0)
                    logging_setlevel(l);
        }
    }
}

static const char * get_time_str(char * buffer) {
    using std::chrono::system_clock;
    auto now = system_clock::now();
    auto fraction = now - std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
    time_t cnow = system_clock::to_time_t(now);
    char str[30];
    strftime(str, sizeof(str), "%F %T", localtime(&cnow));
    sprintf(buffer, "%s.%03d", str, milliseconds.count());
    return buffer;
}

static void logging_vfprintfln(FILE* fp, int level, const char * fmt, va_list vl) {
    // TODO time, headers
    char buffer[30];
    fprintf(fp, "[%s]", get_time_str(buffer));
    fprintf(fp, "[%s] ", get_level_str(level));
    vfprintf(fp, fmt, vl);
    fprintf(fp, "\n");
}

void logging_init() {
    set_output_from_env();
    set_level_from_env();
}

void logging_finalize() {
    if (logging_output_fp) {
        fflush(logging_output_fp);
        fclose(logging_output_fp);
    }
}

void logging_setlevel(int level) {
    infofln("setting logging level to %s", get_level_str(level));
    logging_level = level;
}

void logging_printfln(int level, const char * fmt, ...) {
    va_list vl;
    // logging to stderr
    if (level >= logging_level) {
        va_start(vl, fmt);
        logging_vfprintfln(stderr, level, fmt, vl);
        va_end(vl);
        fflush(stderr);
    }
    // logging to file
    if (logging_output_fp) {
        va_start(vl, fmt);
        logging_vfprintfln(logging_output_fp, level, fmt, vl);
        va_end(vl);
    }
}
