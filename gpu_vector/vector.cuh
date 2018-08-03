#include <vector>
#include <thrust/execution_policy.h>
#include <thrust/device_vector.h>

namespace gpu {

template <typename T>
class vector : public thrust::device_vector<T> {
public:
    vector() {}
    vector(size_t s) { this->resize(s); }
    vector(const std::vector<T> & v) {
        *static_cast<thrust::device_vector<T>*>(this) = v;
    }
    operator std::vector<T>() {
        thrust::host_vector<T> r = *this;
        return std::vector<T>(r.begin(), r.end());
    }

    template <typename Op>
    auto map(Op op) {
        typedef decltype(op((*this)[0])) ret;
        vector<ret> r(this->size());
        thrust::transform(thrust::device, this->begin(), this->end(), r.begin(), op);
        return r;
    }
    template <typename Op>
    auto map_(Op op) {
        thrust::transform(thrust::device, this->begin(), this->end(), this->begin(), op);
        return *this;
    }
    T * data() {
        return this->data();
    }
};

}
