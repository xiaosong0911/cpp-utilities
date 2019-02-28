#pragma once

#include <stdio.h>

class VideoWriter {
public:
    VideoWriter(); // default to "output.mp4" with 24fps
    VideoWriter(const char * filename, int framerate);
    ~VideoWriter();
    void writeFrameRGB(int w, int h, float * p);
    void writeFrameRGB(int w, int h, unsigned char * p);
    int getFrame() { return frame; }
#ifdef __gl_h_
void writeFrameGL(int w, int h) {
    std::vector<unsigned char> buf(w * h * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buf.data());
    writeFrameRGB(w, h, buf.data());
}
#endif
private:
    int frame;
    FILE * fp;
};

