#include <GLFW/glfw3.h>
using namespace std;
class Point {
private:
    int x;
    int y;
public:
    Point(int x, int y);
    int get_x();
    int get_y();
    GLfloat get_gl_x(int width, int height);
    GLfloat get_gl_y(int width, int height);
};