#include <vector>
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include "point.h"
using namespace std;
class Ellipsoid{
private:
    int point_count;
    GLfloat a;
    GLfloat b;
    GLfloat c;
    int count_of_width_segments;
    int count_of_height_segments;
    vector<GLfloat> vertexes;
    vector<GLfloat> colors;
    void make_vertexes();
    void convert_point_to_vertexes(Point point, Point color);
public:
    Ellipsoid(int count_of_width_segments, int count_of_height_segments, GLfloat a, GLfloat b, GLfloat c);
    void make_ellipsoid();
    void remake_ellipsoid(int step_phi, int step_teta);
};