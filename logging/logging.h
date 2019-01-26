#ifndef LOGGING_H
#define LOGGING_H

enum {
    LOGGING_DEBUG = 0,
    LOGGING_INFO,
    LOGGING_WARNING,
    LOGGING_ERROR,
    LOGGING_LEVEL_COUNT // level count in total
};

void logging_init();
void logging_finalize();

void logging_setlevel(int level);

void logging_printfln(int level, const char * fmt, ...);

// disable debug logging with NDEBUG macro
#ifdef NDEBUG
#define debugfln(...)   do {} while(0)
#else
#define debugfln(...)   logging_printfln(LOGGING_DEBUG,   __VA_ARGS__)
#endif

#define infofln(...)    logging_printfln(LOGGING_INFO,    __VA_ARGS__)
#define warningfln(...) logging_printfln(LOGGING_WARNING, __VA_ARGS__)
#define errorfln(...)   logging_printfln(LOGGING_ERROR,   __VA_ARGS__)

#endif
