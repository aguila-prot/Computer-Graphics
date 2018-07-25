#include "ellipsoid.h"
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void make_perspective();
void resizeHandler(GLFWwindow *window, int w, int h);
int dAngle = 0, rAngle = 0, uAngle=0, width = 800, height=800;
GLfloat x_centre=0, y_centre=0, z_centre=0, scaler=1;
double x_pos, y_pos;
unsigned int count_of_width_segments=10, count_of_height_segments=11;
bool modePolygon = false, remake_flag = false;
int main(){
    if(!glfwInit()){
        cout << "Failed in init";
        return -1;
    }
    GLFWwindow *window = glfwCreateWindow(800, 800, "Lab3", nullptr, nullptr);
    if(!window){
        cout << "Failed in create window";
        glfwTerminate();
        return -1;
    }
    GLfloat Vertex[24][3] = {
            {-1.5f,-1.5f,-1.5f},
            {-1.f,-1.5f,-1.5f},
            {-1.f,-1.f,-1.5f},
            {-1.5f,-1.f,-1.5f},
            {-1.5f,-1.5f,-1.f},
            {-1.f,-1.5f,-1.f},
            {-1.f,-1.f,-1.f},
            {-1.5f,-1.f,-1.f},
            {-1.5f,-1.5f,-1.5f},
            {-1.f,-1.5f,-1.5f},
            {-1.f,-1.5f,-1.f},
            {-1.5f,-1.5f,-1.f},
            {-1.5f,-1.f,-1.5f},
            {-1.f,-1.f,-1.5f},
            {-1.f,-1.f,-1.f},
            {-1.5f,-1.f,-1.f},
            {-1.5f,-1.5f,-1.5f},
            {-1.5f,-1.5f,-1.f},
            {-1.5f,-1.f,-1.f},
            {-1.5f,-1.f,-1.5f},
            {-1.f,-1.5f,-1.5f},
            {-1.f,-1.5f,-1.f},
            {-1.f,-1.f,-1.f},
            {-1.f,-1.f,-1.5f}
    };
    GLfloat Colors[24][3] = {
            {1.f ,0.f ,0.f},
            {1.f ,0.f ,0.f},
            {1.f ,0.f ,0.f},
            {1.f ,0.f ,0.f},
            {0.f ,0.f ,1.f},
            {0.f ,0.f ,1.f},
            {0.f ,0.f ,1.f},
            {0.f ,0.f ,1.f},
            {0.f ,1.f ,0.f},
            {0.f ,1.f ,0.f},
            {0.f ,1.f ,0.f},
            {0.f ,1.f ,0.f},
            {1.f ,0.f ,1.f},
            {1.f ,0.f ,1.f},
            {1.f ,0.f ,1.f},
            {1.f ,0.f ,1.f},
            {0.f ,1.f ,1.f},
            {0.f ,1.f ,1.f},
            {0.f ,1.f ,1.f},
            {0.f ,1.f ,1.f},
            {1.f ,1.f ,0.f},
            {1.f ,1.f ,0.f},
            {1.f ,1.f ,0.f},
            {1.f ,1.f ,0.f}
    };
    Ellipsoid *ellipsoid = new Ellipsoid(count_of_width_segments, count_of_height_segments, 2, 2.5, 2);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, resizeHandler);
    make_perspective();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
    while(!glfwWindowShouldClose(window)){
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, Vertex);
        glColorPointer(3, GL_FLOAT, 0, Colors);
        glDrawArrays(GL_QUADS, 0, 24);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glPushMatrix();
        glTranslated(x_centre, y_centre, z_centre);
        glRotated(dAngle, 0.f, 1.f, 0.f);
        glRotated(rAngle, 1.f, 0.f, 0.f);
        glRotated(uAngle, 0.f, 0.f, 1.f);
        glScaled(scaler, scaler, scaler);
        if(remake_flag){
            remake_flag = false;
            ellipsoid->remake_ellipsoid(count_of_width_segments, count_of_height_segments);
        }
        ellipsoid->make_ellipsoid();
        //std::this_thread::sleep_for(std::chrono::milliseconds(0));
        glPopMatrix();
        glLoadIdentity();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (modePolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        modePolygon = !modePolygon;
    }
    if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        uAngle += 5;
    if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        dAngle += 5;
    if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
        rAngle += 5;
    if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        y_centre += 0.05;
    if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        y_centre -= 0.05;
    if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        x_centre -= 0.05;
    if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        x_centre += 0.05;
    if(key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
        scaler += 0.05;
    if(key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT))
        scaler -= 0.05;
    if(key == GLFW_KEY_U || key == GLFW_KEY_I || key == GLFW_KEY_J || key == GLFW_KEY_K)
        remake_flag = true;
    if(key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT))
        count_of_width_segments += 1;
    if(key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
        count_of_width_segments -= 1;
    if(key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
        count_of_height_segments += 2;
    if(key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
        count_of_height_segments -= 2;
}
void make_perspective(){
    glMatrixMode(GL_PROJECTION);
    double coff[] = {
            1, 0, 0, 0.2,
            0, 1, 0, 0.2,
            0, 0, -1, 0,
            0, 0, 0, 3.5
    };
    double phi = -1;
    double m1[] = {
            cos(phi) , 0 , sin(phi) , 0,
            0 , 1 , 0 , 0,
            -sin(phi), 0 , cos(phi) , 0,
            0 , 0 , 0, 1
    };
    glLoadMatrixd(coff);
    glMultMatrixd(m1);
}
void resizeHandler(GLFWwindow *window, int w, int h){
    width = w;
    height = h;
    if(w > h)
        glViewport(0, 0, h, h);
    else
        glViewport(0, 0, w, w);
    glLoadIdentity();
}