#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>

class Buffer {
public:
    Buffer();
    void draw_pixel(int x, int y, float r, float g, float b);
    void clear_data();
    void resize(int w, int h);
    void make_acc_buffer();
    void draw();
    void init(int width, int height);
    int width;
    int height;
    size_t size;
    float *pixels;
    float *acc_buffer;
};