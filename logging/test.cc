#include "logging.h"

void test_printf() {
    const char * test = "hello";
    debugfln("%s", test);
    infofln("%s", test);
    warningfln("%s", test);
    errorfln("%s", test);
}

void test_performance() {
    for (int i = 0; i < 10000; i++) {
        debugfln("%d", i);
    }
}

void test() {
    test_printf();
    test_performance();
}

int main() {
    logging_init();
    logging_setlevel(LOGGING_DEBUG);
    test();
    logging_finalize();
}
