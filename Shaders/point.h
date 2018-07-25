#include "include.h"
class Point{
private:
    GLfloat x, y, z;
public:
    Point(GLfloat x, GLfloat y, GLfloat z);
    Point(GLfloat x, GLfloat y);
    GLfloat get_x();
    GLfloat get_y();
    GLfloat get_z();
};