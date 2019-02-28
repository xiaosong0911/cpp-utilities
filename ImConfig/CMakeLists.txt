find_package(tinyxml2 CONFIG REQUIRED)

add_library(ImConfig
    ImConfig.h
    ImConfig.cc
    )
target_link_libraries(ImConfig
    tinyxml2::tinyxml2
    )

add_executable(imconfig_test
    test.cc
    )
target_link_libraries(imconfig_test
    ImConfig
    )