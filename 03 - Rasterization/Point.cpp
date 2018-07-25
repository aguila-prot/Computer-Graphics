#include "Point.h"
#include <algorithm>
#include <iostream>
Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}
int Point::get_x() {
    return x;
}
int Point::get_y() {
    return y;
}
GLfloat Point::get_gl_x(int width, int height) {
    GLfloat ration = (width > height) ? 1 : (GLfloat)height/(GLfloat)width;
    return ((GLfloat)x*ration/((GLfloat)max(height, width)*0.5f))-1;
}
GLfloat Point::get_gl_y(int width, int height) {
    GLfloat ration = (width < height) ? 1 : (GLfloat)width/(GLfloat)height;
    return -1.f*(((GLfloat)y*ration/((GLfloat)max(height, width)*0.5f)) - 1);
}