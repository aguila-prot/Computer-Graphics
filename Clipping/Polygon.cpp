#include "Polygon.h"
#include<iostream>
using namespace std;
Polygon::Polygon(int type, int id) {
    if(type == 0)
        r = g = b = 1;
    if(type == 1)
        r = g = 0, b = 1;
    if(type == 2)
        r = 1, g = 1, b = 0;
    if(type == 3)
        r = 0, g = 1, b = 1;
    is_finish = false;
    has_inter = false;
    this->id = id;
    this->type = type;
}
void Polygon::add_point(int x, int y, int sub_count, int clip_count) {
    Point c = Point(x, y, window_width, window_height, false, false, sub_count, clip_count);
    points.emplace_back(c);
    drawing_edges.push_back(c.get_gl_x());
    drawing_edges.push_back(c.get_gl_y());
}

void Polygon::draw() {
    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, drawing_edges.data());
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) drawing_edges.size() / 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    if(is_finish) return;
    Point p = Point((int)xpos, (int)ypos, window_width, window_height, false, false, -1, -1);
    if(drawing_edges.size() >= 2){
        glBegin(GL_LINES);
        glColor3f(r, g, b);
        glVertex2f(drawing_edges[0], drawing_edges[1]);
        glVertex2f(p.get_gl_x(), p.get_gl_y());
        if(drawing_edges.size() >= 4) {
            glColor3f(1, 0, 0);
            glVertex2f(drawing_edges[drawing_edges.size() - 2], drawing_edges[drawing_edges.size() - 1]);
            glVertex2f(p.get_gl_x(), p.get_gl_y());
        }
        glEnd();
    }
}
void Polygon::clear_data() {
    points.clear();
    drawing_edges.clear();
    is_finish = false;
}
void Polygon::set_current_cursor_position(double xpos, double ypos) {
    this->xpos = xpos;
    this->ypos = ypos;
}
void Polygon::set_current_framebuffer_size(int window_width, int window_height, int frame_width, int frame_height) {
    this->frame_width = frame_width;
    this->frame_height = frame_height;
    this->window_width = window_width;
    this->window_height = window_height;
}
void Polygon::end_draw() {
    is_finish = true;
    drawing_edges.push_back(drawing_edges[0]);
    drawing_edges.push_back(drawing_edges[1]);
}
bool Polygon::get_finish() {
    return is_finish;
}
int Polygon::get_type() {
    return type;
}
void Polygon::set_inter() {
    has_inter = true;
}
int Polygon::get_id() {
    return id;
}
bool Polygon::get_inter() {
    return has_inter;
}