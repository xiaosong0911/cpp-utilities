#include <cstdio>
#include "cartesian_grid.h"
#include "CompactSearch.cuh"

void test_cartesian_grid() {
    cartesian_grid3 g;
    g.setRange(
            {-1,-1,-1},
            { 1, 1, 1},
            0.1);
    glm::ivec3 c = g.cellFromPos({0, 1, .1});
    printf("%d, %d, %d\n", c.x, c.y, c.z);
}

void test_compact_search() {
    thrust::device_vector<glm::fvec3> pos;
    {
        std::vector<glm::fvec3> p;
        FILE * fp = fopen("p.txt", "r");
        float x,y,z;
        while(3 == fscanf(fp, "%f %f %f\n", &x, &y, &z)) {
            p.push_back({x,y,z});
        }
        fclose(fp);
        pos = p;
    }
    CompactSearch cs;
    cs.setRange(
            {-1,-1,-1},
            { 1, 1, 1},
            0.1);
    int num;
    cs.sort(pos, num);
}

int main() {
    test_cartesian_grid();
    test_compact_search();
}
