#pragma once

#include <array>
#include <vector>

template <size_t s> constexpr float poly_calc(const std::array<float, s> & a, float x);
template <size_t s> constexpr std::vector<float> poly_solve(const std::array<float, s> &a);

template <size_t s>
struct solver {
    static constexpr std::vector<float> solve(const std::array<float, s> &a) {
        std::array<float, s - 1> a_{};
        if (a[s - 1] == 0) {
            for (size_t i = 0; i < s - 1; i++)
                a_[i] = a[i];
            return solver<s - 1>::solve(a_);
        }
        for (size_t i = 1; i < s; i++)
            a_[i - 1] = a[i] * (float)i;
        std::vector<float> stationary = solver<s - 1>::solve(a_);
        // check roots
        std::vector<float> solutions;
        float l = 0, h = 0;
        if (!stationary.empty()) l = stationary[0];
        for (float d = 1;; d *= 2) {
            l -= d;
            if ((s % 2 == 0) == (a[s - 1] > 0)) {
                if (poly_calc(a, l) < 0) break;
            } else {
                if (poly_calc(a, l) > 0) break;
            }
        }
        for (float x : stationary) {
            h = x;
            if (poly_calc(a, h) * poly_calc(a, l) < 0)
                solutions.push_back(binary_solve(a, l, h));
            l = x;
        }
        for (float d = 1;; d *= 2) {
            h += d;
            if (a[s - 1] > 0) {
                if (poly_calc(a, h) > 0) break;
            } else {
                if (poly_calc(a, h) < 0) break;
            }
        }
        if (poly_calc(a, h) * poly_calc(a, l) < 0)
            solutions.push_back(binary_solve(a, l, h));
        return solutions;
    }
    static constexpr float binary_solve(const std::array<float, s> &a, float xl, float xh) {
        float yl = poly_calc(a, xl);
        float yh = poly_calc(a, xh);
        assert(yl * yh < 0);
        for (;;) {
            float xm = (xl + xh) * 0.5f;
            const float e = (xm > 0 ? xm : -xm) * 1e-6f;
            if (xh - xl <= e) return xm;
            float ym = poly_calc(a, xm);
            if (ym * yl > 0) {
                xl = xm; yl = poly_calc(a, xl);
            } else {
                xh = xm; yh = poly_calc(a, xl);
            }
        }
    }
};

template <>
struct solver<1> {
    static std::vector<float> solve(const std::array<float, 1> &a) {
        return {};
    }
};

template <size_t s>
constexpr float poly_calc(const std::array<float, s> & a, float x) {
    size_t n = s; float r = 0;
    while (n-- > 0) r = r * x + a[n];
    return r;
}

template <size_t s>
constexpr std::vector<float> poly_solve(const std::array<float, s> &a) {
    return solver<s>::solve(a);
}

#ifdef IMGUI_VERSION
#ifdef POLYNOMIAL_TEST
inline void test() {
    static std::array<float, 6> a{-0.1, 0.5, 0, -0.5, 0, 0.1};
    for (int i = 0; i < a.size(); i++) {
        char l[32];
        sprintf(l, "%d", i);
        ImGui::DragFloat(l, &a[i], 0.001, -1, 1);
    }
    ImDrawList *dl = ImGui::GetOverlayDrawList();
    ImVec2 ds = ImGui::GetIO().DisplaySize;
    ImU32 col = IM_COL32(200, 200, 200, 255);
    dl->AddLine(ImVec2(ds.x / 2, 0), ImVec2(ds.x / 2, ds.y), col);
    dl->AddLine(ImVec2(0, ds.y / 2), ImVec2(ds.x, ds.y / 2), col);
    for (float x = -5; x < 5; x += 0.02) {
        float y = poly_calc(a, x);
        float sx = (x - (-5)) / (5 - (-5)) * ImGui::GetIO().DisplaySize.x;
        float sy = (5 - y) / (5 - (-5)) * ImGui::GetIO().DisplaySize.y;
        dl->PathLineTo(ImVec2(sx, sy));
    }
    col = IM_COL32(0, 0, 0, 255);
    dl->PathStroke(col, false);
    {
        auto s = poly_solve(a);
        ImGui::Text("real roots %d.", s.size());
        for (float x : s) {
            float sx = (x - (-5)) / (5 - (-5)) * ImGui::GetIO().DisplaySize.x;
            dl->AddCircle(ImVec2(sx, ds.y / 2), 8, col);
        }
    }
}
#endif
#endif