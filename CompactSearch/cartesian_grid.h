#pragma once

#include <glm/glm.hpp>

/* compatible with cuda */
#ifdef __NVCC__
#define FUNCTION_HEADER __host__ __device__
#else
#define FUNCTION_HEADER
#endif

struct cartesian_grid3 {
    using fvec3 = glm::fvec3;
    using ivec3 = glm::ivec3;
    fvec3 min;
    ivec3 res;
    float cubeSize;
    FUNCTION_HEADER
    void setRange(fvec3 min_, fvec3 max_, float cubeSize_) {
        min = min_;
        cubeSize = cubeSize_;
        fvec3 extent = (max_ - min) / cubeSize;
        res.x = ceil(extent.x);
        res.y = ceil(extent.y);
        res.z = ceil(extent.z);
    }
    FUNCTION_HEADER
    ivec3 cellFromPos (fvec3 p) const {
        fvec3 offset = (p - min) / cubeSize;
        ivec3 cell;
        cell.x = floor(offset.x);
        cell.y = floor(offset.y);
        cell.z = floor(offset.z);
        return cell;
    }
    FUNCTION_HEADER
    fvec3 integerPoint(ivec3 c) const {
        return fvec3(c) * cubeSize + min;
    }
    FUNCTION_HEADER
    fvec3 cellCentor(ivec3 c) const {
        return integerPoint(c) + cubeSize * 0.5f;
    }
    FUNCTION_HEADER
    int cellNumber() const {
        return cellHash(res - 1) + 1;
    }
    FUNCTION_HEADER
    int cellHash(ivec3 c) const {
        if (c.x < 0 || c.x >= res.x) return -1;
        if (c.y < 0 || c.y >= res.y) return -1;
        if (c.z < 0 || c.z >= res.z) return -1;
        return c.x + res.x * (c.y + res.y * c.z);
    }
};

#undef FUNCTION_HEADER
