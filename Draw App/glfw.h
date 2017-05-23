//
//  glfw.h
//  Draw App
//
//  Created by TA on 22/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#ifndef glfw_h
#define glfw_h

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Glfw {
public:
    Glfw();
    ~Glfw();
    int width();
    int height();
    float aspect_ratio();
    bool mouse_down() const;
    GLFWwindow* window() const;
private:
    int _width, _height;
    GLFWwindow* glfw_window;
    static bool is_mouse_down;
    static void key_callback(GLFWwindow*, int, int, int, int);
    static void mouse_button_callback(GLFWwindow*, int, int, int);
    static void cursor_position_callback(GLFWwindow*, double, double);
};

#endif
