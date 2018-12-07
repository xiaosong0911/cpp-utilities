#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "VideoWriter.h"

using namespace glm;

class ShaderToy {
protected:
    vec3 iResolution;
    float iTime;
public:
    void setTime(float t) {iTime = t;}
    void setRes(vec3 r) {iResolution = r;}
    virtual void mainImage(vec4 & fragColor, vec2 fragCoord) const = 0;
};

inline float random(vec2 st) {
    return fract(sin(dot(st + 1.f, vec2(12.9898,78.233)))*
        43758.5453123);
}

inline float noise (vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0f - 2.0f * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define OCTAVES 4
inline float fbm (vec2 st) {
    // Initial values
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st = st * 2.f + 2.f;
        amplitude *= .5f;
    }
    return value;
}

inline float taichi(vec2 uv) {
        float half = max(length(uv) - 1, -uv.x);
        float fish = max(min(half, length(uv-vec2(0,0.5))-0.5f), 0.5f-length(uv-vec2(0,-0.5)));
        float eye = min(max(fish, 0.125f-length(uv-vec2(0,0.5))), length(uv-vec2(0,-0.5))-0.125f);
        return eye;
}

class Example : public ShaderToy {
public:
    virtual void mainImage(vec4 & fragColor, vec2 fragCoord) const override {
        vec2 uv = fragCoord * 2.f * vec2(iResolution) / iResolution.y ;
        {
            float r = random(uv);
            float r_fbm = fbm((uv+iTime*0.5f) * 2.f);
            fragColor = vec4(vec3(0.8,0.9,1)*r_fbm, 1);
        }
        {
            float theta = iTime * 0.5f * 2 * M_PI;
            float dist = taichi(mat2(cos(theta), -sin(theta), sin(theta), cos(theta)) * uv * 2.f) * 0.5f;
            float grey = smoothstep(-2.f/iResolution.y, 2.f/iResolution.y, dist);
            float mask = smoothstep(-2.f/iResolution.y, 2.f/iResolution.y, length(uv)-0.5f);
            fragColor = vec4(vec3(grey) * (1-mask) + vec3(fragColor) * mask, 1);
        }
    }
};

int main(int argc, char ** argv) {
    int width = 640 / 4;
    int height = 480 / 4;
    int frames = 24 * 2;
    unsigned char * buf = (unsigned char *) malloc(width * height * 3 * sizeof(unsigned char));
    VideoWriter vw;
    Example st;
    st.setRes(vec3(width, height, 1));
    for (int frame = 0; frame < frames; frame++) {
        printf("\rworking with frame %d", frame);
        fflush(stdout);
        st.setTime(frame/24.f);
        std::vector<vec3> buf(width * height);
#pragma omp parallel for
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++) {
                vec2 uv = vec2(x/float(width - 1) - 0.5f, 1-y/float(height - 1)-0.5f);
                vec4 color;
                st.mainImage(color, uv);
                buf[x + y * width] = fvec3(color);
            }
        vw.writeFrameRGB(width, height, &buf[0][0]);
    }
    free(buf);
}
