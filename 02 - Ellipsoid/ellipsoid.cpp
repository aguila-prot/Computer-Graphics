#include "ellipsoid.h"
Ellipsoid::Ellipsoid(int count_of_width_segments, int count_of_height_segments, GLfloat a, GLfloat b, GLfloat c) {
    this->count_of_width_segments = count_of_width_segments;
    this->count_of_height_segments = count_of_height_segments;
    this->point_count = 0;
    this->a = a;
    this->b = b;
    this->c = c;
    make_vertexes();
}
void Ellipsoid::convert_point_to_vertexes(Point point, Point color) {
    colors.push_back(color.get_x());
    colors.push_back(color.get_y());
    colors.push_back(color.get_z());
    vertexes.push_back(point.get_x());
    vertexes.push_back(point.get_y());
    vertexes.push_back(point.get_z());
}
void Ellipsoid::make_vertexes() {
    unsigned int max_count = 10000, k = 0;
    double n = 360, m = 180, p = 90, pi = 3.14159265;
    double deg = pi/m;
    double step_teta = n/((double)count_of_height_segments - 1.0);
    double step_phi = m/((double)count_of_width_segments - 1.0);
    GLfloat x, y, z;
    vector<Point> level1(max_count, Point(0, 0, 0)), level2(max_count, Point(0, 0, 0));
    for(double i=0; i<=n;) {
        k=0;
        for(double j=-p; j<=p; k++) {
            x = a * (GLfloat)(sin(deg * i) * cos(deg * j));
            y = b * (GLfloat)(sin(deg * i) * sin(deg * j));
            z = c * (GLfloat)cos(deg * i);
            if(i == 0)
                level2[k] = Point(x, y, z);
            else {
                level1[k] = level2[k];
                level2[k] = Point(x, y, z);
            }
            j = (j < p && j + step_phi > p) ? p : j + step_phi;
        }
        i = (i < n && i + step_teta > n) ? n : i + step_teta;
        if(i == step_teta) continue;
        k=0;
        for(double j=-p; j<p;j+=step_phi, k++){
            double a = fabs(min(i, j)), b = fabs(max(i, j));
            convert_point_to_vertexes(level1[k], Point((GLfloat)sin(a/b), (GLfloat)(a/b), (GLfloat)cos(a/b)));
            convert_point_to_vertexes(level2[k], Point((GLfloat)cos(a/b), (GLfloat)(a/b), (GLfloat)sin(a/b)));
            convert_point_to_vertexes(level1[k+1], Point((GLfloat)sin(a/b), (GLfloat)(a/b), (GLfloat)cos(a/b)));
            convert_point_to_vertexes(level2[k+1], Point((GLfloat)cos(a/b), (GLfloat)(a/b), (GLfloat)sin(a/b)));
            convert_point_to_vertexes(level2[k], Point((GLfloat)sin(a/b), (GLfloat)(a/b), (GLfloat)cos(a/b)));
            convert_point_to_vertexes(level1[k+1], Point((GLfloat)cos(a/b), (GLfloat)(a/b), (GLfloat)sin(b)));
            point_count+=6;
        }
    }
}
void Ellipsoid::make_ellipsoid() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexes.data());
    glColorPointer(3, GL_FLOAT, 0, colors.data());
    glDrawArrays(GL_TRIANGLES, 0, point_count);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
void Ellipsoid::remake_ellipsoid(int count_of_width_segments, int count_of_height_segments) {
    this->count_of_width_segments = count_of_width_segments;
    this->count_of_height_segments = count_of_height_segments;
    this->point_count = 0;
    vertexes.clear();
    colors.clear();
    make_vertexes();
}