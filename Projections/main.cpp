#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
using namespace std;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void make_perspective();
void resizeHandler(GLFWwindow *window, int width, int height);
void make_cube(GLfloat Colors[24][3], GLfloat Vertex[24][3]);
int dAngle = 0, rAngle = 0, uAngle=0, lAngle=0;
GLfloat x_centre=0, y_centre=0, z_centre=0, scaler=1;
int width = 800, height = 800;
bool modePolygon = false;

int main(){
    GLfloat Vertex1[24][3] = {
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
    GLfloat Vertex2[24][3] = {
            {0.5f,0.5f,-0.5f},
            {-0.5f,0.5f,-0.5f},
            {-0.5f,-0.5f,-0.5f},
            {0.5f,-0.5f,-0.5f},
            {0.5f,0.5f,0.5f},
            {-0.5f,0.5f,0.5f},
            {-0.5f,-0.5f,0.5f},
            {0.5f,-0.5f,0.5f},
            {0.5f,-0.5f,0.5f},
            {-0.5f,-0.5f,0.5f},
            {-0.5f,-0.5f,-0.5f},
            {0.5f,-0.5f,-0.5f},
            {0.5f,0.5f,0.5f},
            {-0.5f,0.5f,0.5f},
            {-0.5f,0.5f,-0.5f},
            {0.5f,0.5f,-0.5f},
            {-0.5f,0.5f,0.5f},
            {-0.5f,0.5f,-0.5f},
            {-0.5f,-0.5f,-0.5f},
            {-0.5f,-0.5f,0.5f},
            {0.5f,0.5f,0.5f},
            {0.5f,0.5f,-0.5f},
            {0.5f,-0.5f,-0.5f},
            {0.5f,-0.5f,0.5f}
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
    if(!glfwInit()){
        cout << "Failed in init";
        return -1;
    }
    GLFWwindow *window = glfwCreateWindow(width, height, "Lab2", nullptr, nullptr);
    if(!window){
        cout << "Failed in create window";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, resizeHandler);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    //glMatrixMode(GL_VIEWPORT);
    //glfwGetFramebufferSize(window, &width, &height);

    glLoadIdentity();
    while(!glfwWindowShouldClose(window)){
        glMatrixMode(GL_MODELVIEW_MATRIX);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        make_cube(Colors, Vertex1);
        glPushMatrix();
        glTranslated(x_centre, y_centre, z_centre);
        glRotated(dAngle, 0.f, -1.f, 0.f);
        glRotated(rAngle, 1.f, 0.f, 0.f);
        glRotated(uAngle, 0.f, 0.f, 1.f);
        glScaled(scaler, scaler, scaler);
        make_cube(Colors, Vertex2);
        glPopMatrix();
        glLoadIdentity();
        make_perspective();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (modePolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        modePolygon = !modePolygon;
    }
    if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
        uAngle += 5;
    if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
        lAngle += 5;
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
}
void make_perspective(){
    glMatrixMode(GL_PROJECTION);
    double coffs[] = {
            1 , 0 , 0 , 0.5 ,
            0 , 1, 0 , 0.5 ,
            0 , 0 , -1 , 0 ,
            0 , 0 , 0, 4.5
    };
    double phi = -1;
    double m[] = {
            cos(phi) , 0 , sin(phi) , 0,
            0 , 1 , 0 , 0,
            -sin(phi), 0 , cos(phi) , 0,
            0 , 0 , 0, 1
    };
    glLoadMatrixd(coffs);
    glMultMatrixd(m);
}
void make_cube(GLfloat Colors[24][3], GLfloat Vertex[24][3]){
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, Vertex);
    glColorPointer(3, GL_FLOAT, 0, Colors);
    glDrawArrays(GL_QUADS, 0, 24);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
void resizeHandler(GLFWwindow *window, int w, int h){
    width = w;
    height = h;
    if(w > h){
        glViewport(0, 0, h, h);
    }else{
        glViewport(0, 0, w, w);
    }
    glLoadIdentity();
    make_perspective();
}