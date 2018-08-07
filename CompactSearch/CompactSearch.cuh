#pragma once

#include <thrust/device_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/gather.h>
#include <thrust/scatter.h>

#include "cartesian_grid.h"

// call in gpu kernel
struct CompactSearchCUDA : cartesian_grid3 {
    CompactSearchCUDA(
            cartesian_grid3 c,
            int * pId,
            int * cOff
            ) :
        cartesian_grid3(c),
        particleId(pId),
        cellOffset(cOff)
    {}
    int * particleId;
    int * cellOffset;
    template <typename Op>
    __device__ void for_each_neighbor_offset(
            fvec3 p, Op op) const {
        const ivec3 cell = cellFromPos(p);
        ivec3 min = cell - 1;
        ivec3 max = cell + 1;
        if (min.x < 0) min.x = 0;
        if (min.y < 0) min.y = 0;
        if (min.z < 0) min.z = 0;
        if (max.x >= res.x) max.x = res.x - 1;
        if (max.y >= res.y) max.y = res.y - 1;
        if (max.z >= res.z) max.z = res.z - 1;
        for (int z = min.z; z <= max.z; z++)
            for (int y = min.y; y <= max.y; y++)
                for (int x = min.x; x <= max.x; x++) {
                    const int cell = cellHash({x,y,z});
                    if (cell >= 0) {
                        const int begin = cellOffset[cell];
                        const int end = cellOffset[cell+1];
                        for (int offset = begin; offset < end; offset++)
                            op(offset);
                    }
                }
    }
};

class CompactSearch {
    cartesian_grid3 cGrid;
    thrust::device_vector<int> particleId;
    thrust::device_vector<int> cellOffset;
public:
    void sort(const thrust::device_vector<glm::fvec3> & pos, int & particleNumberInRange);
    template <typename T> void scatter(thrust::device_vector<T> & v);
    template <typename T> void gather(thrust::device_vector<T> & v);
    void setGrid(cartesian_grid3 cg) {cGrid = cg;}
    void setRange(glm::fvec3 min, glm::fvec3 max, float cubeSize) {
        cartesian_grid3 cg;
        cg.setRange(min, max, cubeSize);
        setGrid(cg);
    }
    struct CompactSearchCUDA getCUDA() {
        return CompactSearchCUDA(
                cGrid,
                particleId.data().get(),
                cellOffset.data().get());
    }
};

template <typename T>
void CompactSearch::scatter(thrust::device_vector<T> & v) {
    assert(v.size() == particleId.size());
    thrust::device_vector<T> buf(std::move(v));
    v.resize(buf.size());
    thrust::scatter(thrust::device,
            particleId.begin(), particleId.end(),
            buf.begin(),
            v.begin());
}

template <typename T>
void CompactSearch::gather(thrust::device_vector<T> & v) {
    assert(v.size() == particleId.size());
    thrust::device_vector<T> buf(std::move(v));
    v.resize(buf.size());
    thrust::gather(thrust::device,
            particleId.begin(), particleId.end(),
            buf.begin(),
            v.begin());
}
