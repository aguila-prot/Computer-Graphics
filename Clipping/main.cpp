#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>
#include "WeilerAtherton.h"
using namespace std;
int width = 800, height = 800;
int count_clipper = 0, count_subject = 0;
int frame_width, frame_height;
int type = 0;
double xpos, ypos;
void window_size_callback(GLFWwindow * window, int w , int h);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void init();
vector<Polygon> subject;
vector<Polygon> clipper;
vector<Polygon> result;
int main() {
    if(!glfwInit()){
        cout << "Failed init";
        return -1;
    }
    GLFWwindow *window = glfwCreateWindow(width, height, "Lab5", nullptr, nullptr);
    if(!window){
        cout << "Failde in create window";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    glViewport(0, 0, frame_width, frame_height);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    init();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glfwGetCursorPos(window, &xpos, &ypos);
        if(!result.empty()){
            for(Polygon polygon : result){
                glColor3f(1, 0, 0);
                glBegin(GL_LINE_LOOP);
                for(int i=0; i<polygon.points.size(); i++)
                    glVertex2f(polygon.points[i].get_gl_x(), polygon.points[i].get_gl_y());
                glEnd();
            }
        }
        for (int i = 0; i < subject.size(); i++) {
            subject[i].set_current_cursor_position(xpos, ypos);
            subject[i].draw();
        }
        for (int i = 0; i < clipper.size(); i++) {
            clipper[i].set_current_cursor_position(xpos, ypos);
            clipper[i].draw();
        }
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
void window_size_callback(GLFWwindow * window, int w , int h){
    width = w;
    height = h;
    glfwGetFramebufferSize(window, &frame_width, &frame_height);
    glViewport(0, 0, frame_width, frame_height);
    init();
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_C && action == GLFW_PRESS)
        type = 2;
    if(key == GLFW_KEY_W && action == GLFW_PRESS) {
        type = 4;
        WeilerAtherton weilerAtherton(subject, clipper, width, height);
        result = weilerAtherton.get_result();
    }
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS){
        if(type == 3){
            clipper[clipper.size() - 1].end_draw();
        }
        if(type == 2) {
            clipper[clipper.size()-1].end_draw();
            count_clipper++;
            type = 3;
        }
        if(type == 1){
            subject[subject.size() - 1].end_draw();
        }
        if(type == 0){
            subject[subject.size()-1].end_draw();
            count_subject++;
            type = 1;
        }
    }
    if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS){
        init();
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glfwGetCursorPos(window, &xpos, &ypos);
        if(type == 0) {
            subject[0].add_point((int) xpos, (int) ypos, count_subject, 0);
        }
        if(type == 1) {
            if(subject[subject.size()-1].get_finish()) {
                Polygon polygon(1, count_subject++);
                polygon.set_current_framebuffer_size(width, height, frame_width, frame_height);
                subject.emplace_back(polygon);
            }
            subject[subject.size()-1].add_point((int)xpos, (int)ypos, count_subject, 0);
        }
        if(type == 2) {
            clipper[0].add_point((int) xpos, (int) ypos, 0, count_clipper);
        }
        if(type == 3){
            if(clipper[clipper.size()-1].get_finish()) {
                Polygon polygon(3, count_clipper++);
                polygon.set_current_framebuffer_size(width, height, frame_width, frame_height);
                clipper.emplace_back(polygon);
            }
            clipper[clipper.size()-1].add_point((int)xpos, (int)ypos, 0, count_clipper);
        }
    }
}
void init(){
    for(int i=0; i<subject.size(); i++)
        subject[i].clear_data();
    for(int i=0; i<clipper.size(); i++)
        clipper[i].clear_data();
    subject.clear();
    clipper.clear();
    result.clear();
    count_subject = 0;
    count_clipper = 0;
    type = 0;
    clipper.emplace_back(Polygon(2, count_clipper));
    subject.emplace_back(Polygon(0, count_subject));
    clipper[0].set_current_framebuffer_size(width, height, frame_width, frame_height);
    subject[0].set_current_framebuffer_size(width, height, frame_width, frame_height);
}