#include "ellipsoid.h"
Ellipsoid::Ellipsoid(int count_of_width_segments, int count_of_height_segments, GLfloat a, GLfloat b, GLfloat c, int d) {
    this->count_of_width_segments = count_of_width_segments;
    this->count_of_height_segments = count_of_height_segments;
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    vbo = 0;
    shader = Shader::createShader("vertexShader.txt", "fragmentSader.txt");
    cout << shader.ident << endl;
    glGenBuffers(1, &vbo);
    make_vertexes();
}
void Ellipsoid::convert_point_to_vertexes(Point point, Point t) {
    int l = (d == 0) ? -2 : 2;
    vertexes.push_back(point.get_x());
    vertexes.push_back(point.get_y());
    vertexes.push_back(point.get_z());
    normals.push_back(l*point.get_x());
    normals.push_back(l*point.get_y());
    normals.push_back(l*point.get_z());
    text_coords.push_back(t.get_x());
    text_coords.push_back(t.get_y());
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
	data.clear();
    for (int i = 0, t = 0; i < this->vertexes.size(); i += 3, t+=2)
        this->data.emplace_back(tuple<GLfloat, GLfloat, GLfloat>{ vertexes[i], vertexes[i + 1] , vertexes[i + 2] }, tuple<GLfloat, GLfloat>{ text_coords[t], text_coords[t+1] }, tuple<GLfloat, GLfloat, GLfloat>{ normals[i], normals[i + 1], normals[i + 2] });
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size()* sizeof(Vertex), data.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Ellipsoid::make_ellipsoid() {
    GLfloat matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    shader.use();
    //cout << glGetUniformLocation(shader.ident, "model") << endl <<  glGetAttribLocation(shader.ident, "position") << endl;
    glUniformMatrix4fv(glGetUniformLocation(shader.ident, "model"), 1, GL_FALSE, matrix);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(glGetAttribLocation(shader.ident, "position"));
    glVertexAttribPointer(glGetAttribLocation(shader.ident, "position"), 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(glGetAttribLocation(shader.ident, "vertTexCoord"));
    glVertexAttribPointer(glGetAttribLocation(shader.ident, "vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (const GLvoid*)((3) * sizeof(float)));

    glEnableVertexAttribArray(glGetAttribLocation(shader.ident, "vertNormal"));
    glVertexAttribPointer(glGetAttribLocation(shader.ident, "vertNormal"), 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (const GLvoid*)((3 + 2) * sizeof(GLfloat)));
    glDrawArrays(GL_QUADS, 0, vertexes.size()/3);
    shader.unuse();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /*glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals.data());
    glTexCoordPointer(2, GL_FLOAT, 0, text_coords.data());
    glVertexPointer(3, GL_FLOAT, 0, vertexes.data());

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
}
void Ellipsoid::remake_ellipsoid(int count_of_width_segments, int count_of_height_segments) {
    this->count_of_width_segments = count_of_width_segments;
    this->count_of_height_segments = count_of_height_segments;
    vertexes.clear();
    normals.clear();
    text_coords.clear();
    make_vertexes();
}

Ellipsoid::~Ellipsoid() {
    glDeleteBuffers(1, &vbo);
}