#include <vector>
#include "Point.h"
using namespace std;
class Polygon {
private:
    vector<GLfloat> drawing_edges;
    bool is_finish;
    int type;
    int window_width, window_height, frame_width, frame_height;
    double xpos, ypos;
    double r, g, b;
    int id;
    bool has_inter;
public:
    vector<Point> points;
    Polygon(int type, int id);
    void add_point(int x, int y, int sub_count, int clip_count);
    void set_current_framebuffer_size(int window_width, int window_height, int frame_width, int frame_height);
    void set_current_cursor_position(double xpos, double ypos);
    void clear_data();
    void draw();
    void end_draw();
    bool get_finish();
    bool get_inter();
    int get_type();
    void set_inter();
    int get_id();
};