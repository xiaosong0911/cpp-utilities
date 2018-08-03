add_library(ImConfig
    ImConfig.h
    ImConfig.cc
    )
target_link_libraries(ImConfig
    tinyxml2
    )

add_executable(imconfig_test
    test.cc
    )
target_link_libraries(imconfig_test
    ImConfig
    )
