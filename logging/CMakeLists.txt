add_library(logging
    logging.cc
    logging.h
    )

add_executable(logging_test
    test.cc
    )
target_link_libraries(logging_test
    logging
    )