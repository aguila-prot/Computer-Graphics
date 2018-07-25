#include "Polygon.h"
int width = 800, height = 800;
int fwidth, fheight;
Buffer buffer = Buffer();
Polygon polygon = Polygon(&buffer, width, height);
double xpos, ypos;
void window_size_callback(GLFWwindow * window, int w , int h);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int main(){
    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(width, height, "Lab4", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwGetFramebufferSize(window, &fwidth, &fheight);
    buffer.init(fwidth, fheight);
    polygon.set_current_framebuffer_size(fwidth, fheight);
    glViewport(0, 0, fwidth, fheight);
    glLoadIdentity();
    glRasterPos2d(-1, -1);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetCursorPos(window, &xpos, &ypos);
        polygon.set_current_cursor_position(xpos, ypos);
        glMatrixMode(GL_MODELVIEW);
        polygon.draw();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void window_size_callback(GLFWwindow * window, int w , int h){
    glfwGetFramebufferSize(window, &fwidth, &fheight);
    if(polygon.get_mod()) {
        polygon.resize_callback_after_drawing(width, height, fwidth, fheight, w, h);
    }else{
        polygon.resize_callback_after_raster(width, height, fwidth, fheight, w, h);
    }
    width = w;
    height = h;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        glfwGetCursorPos(window, &xpos, &ypos);
        polygon.add_point((int)xpos, (int)ypos);
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
        polygon.clear_data();
    if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        polygon.end_draw();
    if(key == GLFW_KEY_C && action == GLFW_PRESS)
        polygon.change_mod();
    if(key == GLFW_KEY_F && action == GLFW_PRESS)
        polygon.filter();
}