#include "vector.cuh"

using namespace gpu;

struct Op {
    __host__ __device__
    int operator()(int x) {
        return x * x;
    }
};

int main() {
    std::vector<int> i = {0,1,2,3};
    vector<int> di(i);
    di.map_(Op());
    std::vector<int> r = di;
    for (auto i : r)
        std::cout << i << std::endl;
}
