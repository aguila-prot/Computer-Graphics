#include <vector>
#include <cmath>
#include <iostream>
#include "JsonConfig.h"
#include "shader.h"
#define checkImageWidth 64
#define checkImageHeight 64
#define LIGHT_ELLIPSOID 0
#define ELLIPSOID 1
using namespace std;
struct Vertex{
    GLfloat pos[3];
    GLfloat uv[2];
    GLfloat normal[3];
    Vertex(tuple<GLfloat, GLfloat, GLfloat> pos, tuple<GLfloat, GLfloat> uv = tuple<GLfloat, GLfloat>(), tuple<GLfloat, GLfloat, GLfloat> normal = tuple<GLfloat, GLfloat, GLfloat>()) {
        tie(this->pos[0], this->pos[1], this->pos[2]) = pos;
        tie(this->uv[0], this->uv[1]) = uv;
        tie(this->normal[0], this->normal[1], this->normal[2]) = normal;
    }
};
class Ellipsoid {
private:
    GLfloat a;
    GLfloat b;
    GLfloat c;
    void make_vertexes();
    void convert_point_to_vertexes(Point point, Point t);
    int d;
public:
    GLuint vbo;
    Shader shader;
    int count_of_width_segments;
    int count_of_height_segments;
    vector<GLfloat> vertexes;
    vector<GLfloat> normals;
    vector<GLfloat> text_coords;
    vector<Vertex> data;
    Ellipsoid(int count_of_width_segments, int count_of_height_segments, GLfloat a, GLfloat b, GLfloat c, int d);
    ~Ellipsoid();
    void make_ellipsoid();
    void remake_ellipsoid(int step_phi, int step_teta);
};