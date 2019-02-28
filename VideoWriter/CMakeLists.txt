add_library(VideoWriter
    VideoWriter.cc
    VideoWriter.h
    )

add_executable(VideoWriter-test
    test.cc
    )

target_link_libraries(VideoWriter-test
    VideoWriter
    )