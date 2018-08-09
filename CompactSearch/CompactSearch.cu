#include "CompactSearch.cuh"

#include <thrust/sort.h>
#include <thrust/iterator/discard_iterator.h>

namespace {
struct CalcCellOp : cartesian_grid3 {
    CalcCellOp(cartesian_grid3 c) : cartesian_grid3(c) {}
    __device__ int operator()(fvec3 p) {
        return cellHash(cellFromPos(p));
    }
};

struct OffsetWriter {
    int particleNumber, cellNumber;
    int cellCount;
    const int * cell; const int * index;
    int * cellOffset; 
    __device__ int operator()(int i) { // for cell_i
        int start, end, fill;

        if (i == 0) start = 0;
        else {
            const int c = cell[i-1];
            if (c < 0) start = cellNumber + 1; // do not fill
            else start = c + 1;
        }

        if (i == cellCount) end = cellNumber;
        else end = cell[i];
        if (end < 0) end = cellNumber;

        if (i == cellCount) fill = particleNumber;
        else fill = index[i];

        //printf("%d: start: %d end: %d fill:%d\n", i, start, end, fill);
        for (int cell_id = start; cell_id <= end; cell_id++)
            cellOffset[cell_id] = fill;
        return 0;
    }
};
}

void CompactSearch::sort(const thrust::device_vector<glm::fvec3> & pos, int & particleNumberInRange) {
    // calc cell id
    thrust::device_vector<unsigned int> cellId(pos.size());
    thrust::transform(thrust::device,
            pos.begin(), pos.end(),
            cellId.begin(),
            CalcCellOp(cGrid)
            );
    // set pId from 0 to #particles - 1
    auto count_iter = thrust::make_counting_iterator(0);
    particleId.resize(pos.size());
    thrust::copy(thrust::device, count_iter, count_iter + pos.size(), particleId.begin());
    // sort
    thrust::sort_by_key(thrust::device, cellId.begin(), cellId.end(), particleId.begin());
    // get the first particle for each cell
    thrust::device_vector<int> index(pos.size());
    thrust::copy(thrust::device, count_iter, count_iter + index.size(), index.begin());
    auto end = thrust::unique_by_key(thrust::device, cellId.begin(), cellId.end(), index.begin());
    const int cellCount = end.first - cellId.begin();
    // write offset of each cell
    cellOffset.resize(cGrid.cellNumber() + 1);
    struct OffsetWriter ow;
    ow.particleNumber = pos.size();
    ow.cellNumber = cGrid.cellNumber();
    ow.cellCount = cellCount;
    ow.cell = (int *) cellId.data().get();
    ow.index = index.data().get();
    ow.cellOffset = cellOffset.data().get();

    thrust::transform(thrust::device, count_iter, count_iter + cellCount + 1,
            thrust::make_discard_iterator(),
            ow
            );
    particleNumberInRange = cellOffset[cGrid.cellNumber()];
    //fprintf(stderr, "CompactSearch: %d particles in range\n", particleNumberInRange);
}
