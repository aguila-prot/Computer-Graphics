#include "ellipsoid.h"
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void make_perspective();
void init();
void loadTexture();
void drawStaticCube(float x, float y, float z);
void pl(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, vector<GLfloat> &p);
void resizeHandler(GLFWwindow *window, int w, int h);
JsonConfig config("/Users/macbook/CLionProjects/lab_6_Arya/tosave.json");
GLuint texture[1];
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

int main() {
    if (!glfwInit()) {
        cout << "Failed in init";
        return -1;
    }
    GLFWwindow *window = glfwCreateWindow(config.width, config.height, "Lab6", nullptr, nullptr);
    if (!window) {
        cout << "Failed in create window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, resizeHandler);
    init();
    GLfloat last = 0, current, vx = config.vx, vy = config.vy, vz = config.vz;
    GLfloat speed[] = {config.speed_x, config.speed_y, config.speed_z};
    vector<GLfloat> plates;
    pl(-1, -1, -1, -1, 1, -1, 1, 1, -1, plates);
    pl(1, -1, 1, 1, 1, 1, -1, 1, 1, plates);
    pl(1, -1, -1, 1, 1, -1, 1, 1, 1, plates);
    pl(-1, -1, 1, -1, 1, 1, -1, 1, -1, plates);
    pl(1, 1, 1, 1, 1, -1, -1, 1, -1, plates);
    pl(1, -1, -1, 1, -1, 1, -1, -1, 1, plates);
    bool flag_stop;
    if (!config.modePolygon)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Ellipsoid *ellipsoid = new Ellipsoid(config.count_of_width_segments, config.count_of_height_segments, config.x_radius, config.y_radius, config.z_radius, ELLIPSOID);
    Ellipsoid *light = new Ellipsoid(10, 10, 0.1, 0.1, 0.1, LIGHT_ELLIPSOID);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        current = (GLfloat)glfwGetTime();
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glTranslated(config.light_x_centre, config.light_y_centre, config.light_z_centre);
        glRotated(config.d_light_angle, 0.f, 1.f, 0.f);
        glRotated(config.r_light_angle, 1.f, 0.f, 0.f);
        glRotated(config.u_light_angle, 0.f, 0.f, 1.f);
        GLfloat light4_diffuse[] = {0.5, 0.5, 0.5};
        GLfloat light4_position[] = {0, 0, 0, 1.0};
        GLfloat light4_spot_direction[] = {0, -1, 0};
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light4_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light4_position);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, config.rad);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light4_spot_direction);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, config.intensive);
        light->make_ellipsoid();
        glPopMatrix();
        glPushMatrix();
        glTranslated(config.x_centre, config.y_centre, config.z_centre);
        glRotated(config.dAngle, 0.f, 1.f, 0.f);
        glRotated(config.rAngle, 1.f, 0.f, 0.f);
        glRotated(config.uAngle, 0.f, 0.f, 1.f);
        glScaled(config.scaler, config.scaler, config.scaler);
        if (config.remake_flag) {
            config.remake_flag = false;
            ellipsoid->remake_ellipsoid(config.count_of_width_segments, config.count_of_height_segments);
        }
        ellipsoid->make_ellipsoid();
        if (config.move_flag) {
            flag_stop = true;
            GLfloat dt = last - current;
            vector<GLfloat> vertexes = ellipsoid->vertexes;
            for (int i = 0; i < vertexes.size(); i += 3) {
                vertexes[i] += config.x_centre + speed[0] * dt * vx;
                vertexes[i + 1] += config.y_centre + speed[1] * dt * vy;
                vertexes[i + 2] += config.z_centre + speed[2] * dt * vz;
                if (!flag_stop) break;
                for (int j = 0; j < plates.size(); j += 4) {
                    if (plates[j] * vertexes[i] + plates[j + 1] * vertexes[i + 1] + plates[j + 2] * vertexes[i + 2] +
                        plates[j + 3] > 0) {
                        if (plates[j] != 0 && plates[j + 1] == 0 && plates[j + 2] == 0)
                            vx *= -1;
                        else if (plates[j] == 0 && plates[j + 1] != 0 && plates[j + 2] == 0)
                            vy *= -1;
                        else if (plates[j] == 0 && plates[j + 1] == 0 && plates[j + 2] != 0)
                            vz *= -1;
                        else {
                            flag_stop = false;
                            break;
                        }
                    }
                }
            }
            config.x_centre += speed[0] * dt * vx;
            config.y_centre += speed[1] * dt * vy;
            config.z_centre += speed[2] * dt * vz;
        }
        last = current;
        if (config.texture)
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glLoadIdentity();
        if (config.draw_cube) {
            glDisable(GL_TEXTURE_2D);
            drawStaticCube(1, 1, 1);
            if(config.texture)
                glEnable(GL_TEXTURE_2D);
        }

        glDisable(GL_LIGHT0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (config.modePolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        config.modePolygon = !config.modePolygon;
    }
    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.uAngle += 5;
    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.dAngle += 5;
    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.rAngle += 5;
    if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.d_light_angle += 5;
    if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.r_light_angle += 5;
    if (key == GLFW_KEY_3 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.u_light_angle += 5;
    if (key == GLFW_KEY_4 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        if (config.intensive < 50)
            config.intensive += 0.5;
    if (key == GLFW_KEY_5 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        if (config.intensive >= 0)
            config.intensive -= 0.5;
    if (key == GLFW_KEY_6 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        if (config.rad < 100)
            config.rad += 1;
    if (key == GLFW_KEY_7 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        if (config.rad > 0)
            config.rad -= 1;
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.y_centre += 0.05;
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.y_centre -= 0.05;
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.x_centre -= 0.05;
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.x_centre += 0.05;
    if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.scaler += 0.05;

    if (key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.scaler -= 0.05;


    if (key == GLFW_KEY_V && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.light_y_centre -= 0.05;
    if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.light_y_centre += 0.05;
    if (key == GLFW_KEY_B && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.light_x_centre += 0.05;
    if (key == GLFW_KEY_C && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.light_x_centre -= 0.05;
    if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.light_z_centre += 0.05;
    if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.light_z_centre -= 0.05;
    if (key == GLFW_KEY_U || key == GLFW_KEY_I || key == GLFW_KEY_J || key == GLFW_KEY_K)
        config.remake_flag = true;

    if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        config.count_of_width_segments += 1;
    }
    if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.count_of_width_segments -= 1;
    if (key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        config.count_of_height_segments += 2;
    }
    if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.count_of_height_segments -= 2;

    if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.move_flag = !config.move_flag;
    if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.draw_cube = !config.draw_cube;
    if (key == GLFW_KEY_0 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.texture = !config.texture;
    if (key == GLFW_KEY_9 && (action == GLFW_PRESS || action == GLFW_REPEAT))
        config.save("/Users/macbook/CLionProjects/lab_6_Arya/tosave.json");
}
void make_perspective() {
    glMatrixMode(GL_PROJECTION);
    double coff[] = {
            1, 0, 0, 0.2,
            0, 1, 0, 0.2,
            0, 0, 1, 1,
            0, 0, 0, 3.5
    };
    glLoadMatrixd(coff);
}
void resizeHandler(GLFWwindow *window, int w, int h) {
    if (w > h)
        glViewport(0, 0, h, h);
    else
        glViewport(0, 0, w, w);
    glLoadIdentity();
}
void init() {
    make_perspective();
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_ALPHA);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    loadTexture();
}
void drawStaticCube(float x, float y, float z) {
    glColor3f(1, 1, 1);
    glNormal3f(0, 0, -1);
    glBegin(GL_QUADS);
    glVertex3f(x, -y, -z);
    glVertex3f(x, y, -z);
    glVertex3f(-x, y, -z);
    glVertex3f(-x, -y, -z);
    glEnd();
    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(x, -y, z);
    glVertex3f(x, y, z);
    glVertex3f(-x, y, z);
    glVertex3f(-x, -y, z);
    glEnd();
    glNormal3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(x, -y, -z);
    glVertex3f(x, y, -z);
    glVertex3f(x, y, z);
    glVertex3f(x, -y, z);
    glEnd();
    glNormal3f(-1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(-x, -y, z);
    glVertex3f(-x, y, z);
    glVertex3f(-x, y, -z);
    glVertex3f(-x, -y, -z);
    glEnd();
    glNormal3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f(x, y, z);
    glVertex3f(x, y, -z);
    glVertex3f(-x, y, -z);
    glVertex3f(-x, y, z);
    glEnd();
    glNormal3f(0, -1, 0);
    glBegin(GL_QUADS);
    glVertex3f(x, -y, -z);
    glVertex3f(x, -y, z);
    glVertex3f(-x, -y, z);
    glVertex3f(-x, -y, -z);
    glEnd();
}
void loadTexture() {
    for (int i = 0; i < checkImageHeight; i++) {
        for (int j = 0; j < checkImageWidth; j++) {
            int c=(((i&0x8)==0)^((j&0x8)==0))*255;
            checkImage[i][j][0]=(GLubyte)c;
            checkImage[i][j][1]=(GLubyte)c;
            checkImage[i][j][2]=(GLubyte)c;
            checkImage[i][j][3]=(GLubyte)255;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
}
void pl(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, vector<GLfloat> &p) {
    GLfloat A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
    GLfloat B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
    GLfloat C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    GLfloat D = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));
    p.push_back(A);
    p.push_back(B);
    p.push_back(C);
    p.push_back(D);
}