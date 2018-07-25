#include "Buffer.h"
#include <iostream>
using namespace std;
Buffer::Buffer() {
}
void Buffer::draw_pixel(int x, int y, float r, float g, float b){
    int index;
    index = (x * 3 + (height-y-1)*width*3);
    pixels[index] = r;
    pixels[index + 1] = g;
    pixels[index + 2] = b;
}


void Buffer::resize(int w, int h) {
    width = w;
    height = h;
    size = static_cast<size_t>(width * height * 3);
    pixels = (float*)calloc(size , sizeof(float));
    acc_buffer = (float*)calloc(size , sizeof(float));
}


void Buffer::clear_data(){
    memset(&pixels, 0, sizeof(pixels));
    pixels = (float*)calloc(size, sizeof(float));
    memset(&acc_buffer, 0, sizeof(acc_buffer));
    acc_buffer = (float*)calloc(size, sizeof(float));
}

void Buffer::draw() {
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
}
void Buffer::init(int width, int height) {
    this->width = width;
    this->height = height;
    size = static_cast<size_t>(width * height * 3);
    cout << size << endl;
    pixels = (float*)calloc(size, sizeof(float));
    acc_buffer = (float*)calloc(size, sizeof(float));
}
void Buffer::make_acc_buffer() {
    for(int i=0; i<size; i++)
        acc_buffer[i] = pixels[i];
}