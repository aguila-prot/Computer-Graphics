#include "include.h"
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
class JsonConfig {
public:
    int width, height;
    GLfloat dAngle, rAngle, uAngle, d_light_angle, r_light_angle, u_light_angle;
    GLfloat x_centre, y_centre, z_centre, scaler;
    GLfloat speed_x, speed_y, speed_z;
    GLfloat light_x_centre, light_y_centre, light_z_centre, intensive, rad;
    GLfloat vx, vy, vz;
    float x_radius, y_radius, z_radius;
    unsigned int count_of_width_segments, count_of_height_segments;
    bool modePolygon, move_flag, draw_cube, texture;
    bool remake_flag;
    JsonConfig(string path_to_file);
    void save(string path_to_file);
};