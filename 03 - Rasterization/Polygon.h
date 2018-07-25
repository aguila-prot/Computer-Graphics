#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "Edge.h"
#include "Buffer.h"

using namespace std;
class Polygon {
private:
    double xpos;
    double ypos;
    vector<Point> points;
    vector<Edge> edges;
    vector<GLfloat> gl_points;
    Buffer *buffer;
    bool drawing;
    bool mod;
    int width;
    int height;
    int fwidth;
    int fheight;
    int minx;
    int miny;
    int maxx;
    int maxy;
    void draw_lines();
    void draw_rasterize();
public:
    Polygon(Buffer *buffer, int width, int height);
    static bool cmp(Edge &a,Edge &b);
    void add_edge(Point a, Point b);
    void set_current_cursor_position(double xpos, double ypos);
    void set_current_framebuffer_size(int fwidth, int fheight);
    void add_point(int x, int y);
    void clear_data();
    void draw();
    void resize_callback_after_drawing(int width, int height, int fw, int fh, int w, int h);
    void resize_callback_after_raster(int width, int height, int fw, int fh, int w, int h);
    void end_draw();
    void change_mod();
    bool get_mod();
    void draw_interval(int x1, int x2, int y);
    void add_active_edge(int scan_y, vector<Edge> *active_edges);
    void delete_passive_edge(int scan_y, vector<Edge> *active_edges);
    void find_intersections(vector<Edge>* active_edges, vector<int>* intersections);
    void filter();
};