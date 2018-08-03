find_package(CUDA REQUIRED)

#SET( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} --expt-extended-lambda" )

cuda_add_executable(vector_test
    test.cu
    vector.cuh
    )
