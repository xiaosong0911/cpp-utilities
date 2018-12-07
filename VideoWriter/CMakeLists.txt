find_package(OpenGL)

add_library(VideoWriter
    VideoWriter.cc
    VideoWriter.h
    )

if(OPENGL_FOUND)
    target_link_libraries(VideoWriter
        ${OPENGL_LIBRARIES}
        )
    target_compile_definitions(VideoWriter
        PRIVATE USE_OPENGL
        )
endif()

add_executable(VideoWriter-test
    test.cc
    )

target_link_libraries(VideoWriter-test
    VideoWriter
    )


