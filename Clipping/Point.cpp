#include "Point.h"
#include <algorithm>
#include <iostream>
Point::Point(int x, int y, int width, int height, bool enter, bool exit, int index_subject, int index_clipper) {
    this->pixel_x = x;
    this->pixel_y = y;
    GLfloat ration = (width > height) ? 1 : (GLfloat)height/(GLfloat)width;
    gl_x = ((GLfloat)pixel_x*ration/((GLfloat)max(height, width)*0.5f))-1;
    ration = (width < height) ? 1 : (GLfloat)width/(GLfloat)height;
    gl_y = -1.f*(((GLfloat)pixel_y*ration/((GLfloat)max(height, width)*0.5f)) - 1);
    this->enter = enter;
    this->exit = exit;
    this->index_clipper = index_clipper;
    this->index_subject = index_subject;
}
Point::Point(GLfloat x, GLfloat y, int width, int height, bool enter, bool exit, int index_subject, int index_clipper) {
    this->pixel_x = -1;
    this->pixel_y = -1;
    gl_x = x;
    gl_y = y;
    this->enter = enter;
    this->exit = exit;
    this->index_clipper = index_clipper;
    this->index_subject = index_subject;
}
Point::Point() {
}
int Point::get_pixel_x() {
    return pixel_x;
}
int Point::get_pixel_y() {
    return pixel_y;
}
GLfloat Point::get_gl_x() {
    return gl_x;
}
GLfloat Point::get_gl_y() {
    return gl_y;
}
void Point::change_enter() {
    enter = !enter;
}
void Point::change_exit() {
    exit = !exit;
}
void Point::print() {
    cout << gl_x << " " << gl_y << "\tenter\t" << enter << " exit\t" << exit << " ind_sub\t" << index_subject << " ind_clip\t" << index_clipper << endl;
}