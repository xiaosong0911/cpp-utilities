#include "VideoWriter.h"

#include <vector>
#include <math.h>

#include <assert.h>

#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#endif

VideoWriter::VideoWriter() : VideoWriter("output.mp4", 24) {}

VideoWriter::VideoWriter(const char * filename, int framerate) : frame(0) {
    char cmd[512];
    sprintf(cmd,
            "ffmpeg -y "
            "-loglevel error "
            "-framerate %d -f image2pipe -i - "
            "-vf format=yuv420p "
            "-movflags faststart "
            "\"%s\"",
            framerate, filename);
#ifdef _MSC_VER
    fp = popen(cmd, "wb");
#else
    fp = popen(cmd, "w");
#endif
    assert(fp);
}

VideoWriter::~VideoWriter() {
    pclose(fp);
}

inline
unsigned char uchar_of_float(float f) {
    int i = f * 256;
    if (i < 0) return 0;
    if (i > 255) return 255;
    return i;
}

void VideoWriter::writeFrameRGB(int w, int h, unsigned char * buf) {
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "255\n");
    for (int y = h-1; y >= 0; y--)
        fwrite(&buf[y * w * 3], 3, w, fp);
    fflush(fp);
    frame++;
}

void VideoWriter::writeFrameRGB(int w, int h, float * buf) {
    std::vector<unsigned char> pixels(w * h * 3);
    for (int i = 0; i < w * h * 3; i++)
        pixels[i] = uchar_of_float(buf[i]);
    writeFrameRGB(w, h, pixels.data());
}
