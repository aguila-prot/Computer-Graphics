#include "point.h"
Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
GLfloat Point::get_x() {
    return x;
}
GLfloat Point::get_y() {
    return y;
}
GLfloat Point::get_z() {
    return z;
}