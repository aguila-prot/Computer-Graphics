#include "ellipsoid.h"
#include "optimization.h"
Ellipsoid::Ellipsoid(int count_of_width_segments, int count_of_height_segments, GLfloat a, GLfloat b, GLfloat c, int d) {
    this->count_of_width_segments = count_of_width_segments;
    this->count_of_height_segments = count_of_height_segments;
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;

    make_vertexes();
}
void Ellipsoid::convert_point_to_vertexes(Point point, Point t) {
    int l = (d == 0) ? -2 : 2;
    vertexes.push_back(point.get_x());
    vertexes.push_back(point.get_y());
    vertexes.push_back(point.get_z());
#ifndef LIGHT
    normals.push_back(l*point.get_x());
    normals.push_back(l*point.get_y());
    normals.push_back(l*point.get_z());
#endif
#ifndef TEXTURE
    text_coords.push_back(t.get_x());
    text_coords.push_back(t.get_y());
#endif
}
void Ellipsoid::make_vertexes() {
    int l = (d == 0) ? -2 : 2;
    unsigned int max_count = 10000, k = 0;
    double n = 360, m = 180, p = 90, pi = 3.14159265;
    double deg = pi / m;
    double step_teta = n / ((double) count_of_height_segments);
    double step_phi = m / ((double) count_of_width_segments - 1.0);
    GLfloat x, y, z, sp=(GLfloat)4.16/(GLfloat)count_of_width_segments;
    vector<Point> level1(max_count, Point(0, 0, 0)), level2(max_count, Point(0, 0, 0));
    for (double i = 0; i <= n;) {
        k = 0;
        for (double j = -p; j <= p; k++) {
            x = a * (GLfloat) (sin(deg * i) * cos(deg * j));
            y = b * (GLfloat) (sin(deg * i) * sin(deg * j));
            z = c * (GLfloat) cos(deg * i);

            if (i == 0) {
                level2[k] = Point(x, y, z);
            }
            else {
                level1[k] = level2[k];
                level2[k] = Point(x, y, z);
            }
            j = (j < p && j + step_phi > p) ? p : j + step_phi;
        }
        i = (i < n && i + step_teta > n) ? n : i + step_teta;
        if (i == step_teta) continue;
        k = 0;
        GLfloat t=0;
        for (double j = -p; j < p; j += step_phi, k++, t+=sp) {
            convert_point_to_vertexes(level1[k], Point(t, 0));
            convert_point_to_vertexes(level2[k], Point(t, 0.1));
            convert_point_to_vertexes(level2[k + 1], Point(t+sp, 0.1));
            convert_point_to_vertexes(level1[k + 1], Point(t+sp, 0));
        }
        sp = -sp;
    }


#ifndef ARRAY_OF_VERTEXES
#ifdef DISPLAY_LIST
    l = (d == 0) ? -2 : 2;
    glNewList(d+1, GL_COMPILE);
    for (int i = 0, t=0; i < vertexes.size(); i += 12, t+=8) {
        Point a(vertexes[i], vertexes[i + 1], vertexes[i + 2]);
        Point b(vertexes[i + 3], vertexes[i + 4], vertexes[i + 5]);
        Point c(vertexes[i + 6], vertexes[i + 7], vertexes[i + 8]);
        Point dd(vertexes[i + 9], vertexes[i + 10], vertexes[i + 11]);
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
#ifndef LIGHT
        glNormal3f(l*a.get_x(), l*a.get_y(), l*a.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t], text_coords[t+1]);
#endif
        glVertex3d(a.get_x(), a.get_y(), a.get_z());
#ifndef LIGHT
        glNormal3f(l*b.get_x(), l*b.get_y(), l*b.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t+2], text_coords[t+3]);
#endif
        glVertex3d(b.get_x(), b.get_y(), b.get_z());
#ifndef LIGHT
        glNormal3f(l*c.get_x(), l*c.get_y(), l*c.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t+4], text_coords[t+5]);
#endif
        glVertex3d(c.get_x(), c.get_y(), c.get_z());
#ifndef LIGHT
        glNormal3f(l*dd.get_x(), l*dd.get_y(), l*dd.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t+6], text_coords[t+7]);
#endif
        glVertex3d(dd.get_x(), dd.get_y(), dd.get_z());
        glEnd();
    }
    glEndList();
#endif
#else
    #ifdef DISPLAY_LIST
    glNewList(d+1, GL_COMPILE);
#ifndef LIGHT
    glEnableClientState(GL_NORMAL_ARRAY);
#endif
    glEnableClientState(GL_VERTEX_ARRAY);
#ifndef TEXTURE
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
    glNormalPointer(GL_FLOAT, 0, normals.data());
    glTexCoordPointer(2, GL_FLOAT, 0, text_coords.data());
    glVertexPointer(3, GL_FLOAT, 0, vertexes.data());
    glDrawArrays(GL_QUADS, 0, vertexes.size()/3);
#ifndef LIGHT
    glDisableClientState(GL_NORMAL_ARRAY);
#endif
    glDisableClientState(GL_VERTEX_ARRAY);
#ifndef TEXTURE
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
    glEndList();
    #endif
#endif
}

void Ellipsoid::make_ellipsoid() {
#ifndef ARRAY_OF_VERTEXES
#ifdef DISPLAY_LIST
    glCallList(d+1);
#else
    int l = (d == 0) ? -2 : 2;
    for (int i = 0, t=0; i < vertexes.size(); i += 12, t+=8) {
        Point a(vertexes[i], vertexes[i + 1], vertexes[i + 2]);
        Point b(vertexes[i + 3], vertexes[i + 4], vertexes[i + 5]);
        Point c(vertexes[i + 6], vertexes[i + 7], vertexes[i + 8]);
        Point dd(vertexes[i + 9], vertexes[i + 10], vertexes[i + 11]);
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
#ifndef LIGHT
        glNormal3f(l*a.get_x(), l*a.get_y(), l*a.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t], text_coords[t+1]);
#endif
        glVertex3d(a.get_x(), a.get_y(), a.get_z());
#ifndef LIGHT
        glNormal3f(l*b.get_x(), l*b.get_y(), l*b.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t+2], text_coords[t+3]);
#endif
        glVertex3d(b.get_x(), b.get_y(), b.get_z());
#ifndef LIGHT
        glNormal3f(l*c.get_x(), l*c.get_y(), l*c.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t+4], text_coords[t+5]);
#endif
        glVertex3d(c.get_x(), c.get_y(), c.get_z());
#ifndef LIGHT
        glNormal3f(l*dd.get_x(), l*dd.get_y(), l*dd.get_z());
#endif
#ifndef TEXTURE
        glTexCoord2f(text_coords[t+6], text_coords[t+7]);
#endif
        glVertex3d(dd.get_x(), dd.get_y(), dd.get_z());
        glEnd();
    }
#endif
#else
#ifdef DISPLAY_LIST
    glCallList(d+1);
#else
    #ifndef LIGHT
    glEnableClientState(GL_NORMAL_ARRAY);
#endif
    glEnableClientState(GL_VERTEX_ARRAY);
#ifndef TEXTURE
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
    glNormalPointer(GL_FLOAT, 0, normals.data());
    glTexCoordPointer(2, GL_FLOAT, 0, text_coords.data());
    glVertexPointer(3, GL_FLOAT, 0, vertexes.data());
    glDrawArrays(GL_QUADS, 0, vertexes.size()/3);
#ifndef LIGHT
    glDisableClientState(GL_NORMAL_ARRAY);
#endif
    glDisableClientState(GL_VERTEX_ARRAY);
#ifndef TEXTURE
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
#endif
#endif
}
void Ellipsoid::remake_ellipsoid(int count_of_width_segments, int count_of_height_segments) {
    this->count_of_width_segments = count_of_width_segments;
    this->count_of_height_segments = count_of_height_segments;
    vertexes.clear();
    normals.clear();
    text_coords.clear();
    glDeleteLists(d+1, 1);
    make_vertexes();
}