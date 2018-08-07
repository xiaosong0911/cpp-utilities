find_package(CUDA REQUIRED)

cuda_add_library(CompactSearch
    cartesian_grid.h
    CompactSearch.cu
    CompactSearch.cuh
)

cuda_add_executable(CompactSearch-test
    test.cu
    )
target_link_libraries(CompactSearch-test
    CompactSearch
    )
