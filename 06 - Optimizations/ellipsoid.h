#include <vector>
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_test_images.h>
#include "JsonConfig.h"
#include "point.h"
#define checkImageWidth 64
#define checkImageHeight 64
#define LIGHT_ELLIPSOID 0
#define ELLIPSOID 1
using namespace std;
class Ellipsoid {
private:
    GLfloat a;
    GLfloat b;
    GLfloat c;
    void make_vertexes();
    void convert_point_to_vertexes(Point point, Point t);
    int d;
public:
    int count_of_width_segments;
    int count_of_height_segments;
    vector<GLfloat> vertexes;
    vector<GLfloat> normals;
    vector<GLfloat> text_coords;
    Ellipsoid(int count_of_width_segments, int count_of_height_segments, GLfloat a, GLfloat b, GLfloat c, int d);
    void make_ellipsoid();
    void remake_ellipsoid(int step_phi, int step_teta);
};