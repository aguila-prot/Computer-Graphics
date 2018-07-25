#include <GLFW/glfw3.h>
using namespace std;
class Point {
private:
    int pixel_x, pixel_y;
    GLfloat gl_x, gl_y;
public:
    Point(int x, int y, int width, int height, bool enter, bool exit, int index_subject, int index_clipper);
    Point(GLfloat x, GLfloat y, int width, int height, bool enter, bool exit, int index_subject, int index_clipper);
    Point();
    int get_pixel_x();
    int get_pixel_y();
    GLfloat get_gl_x();
    GLfloat get_gl_y();
    void change_enter();
    void change_exit();
    void print();
    int index_subject, index_clipper;
    bool enter, exit;
};