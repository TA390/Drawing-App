//
//  glfw.cpp
//  Draw App
//
//  Created by TA on 22/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#include "glfw.h"

Glfw::Glfw()
: glfw_window{nullptr}, _width(0), _height(0)
{
    // Initialise GLFW
    if(!glfwInit()){
        throw std::runtime_error("GLFW initialisation failed.");
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Swap the buffer after N frames: The docs recommend 1, default is 0
    glfwSwapInterval(1);
    
    // Create a window
    glfw_window = glfwCreateWindow(800, 600, "Drawing App", nullptr, nullptr);
    if(glfw_window == nullptr) {
        throw std::runtime_error("GLFW failed to create a window.");
    }
    glfwMakeContextCurrent(glfw_window);
    
    // Read the actual width and height given to the window
    glfwGetFramebufferSize(glfw_window, &_width, &_height);
    
    // Set view port to the window dimensions
    glViewport(0, 0, _width, _height);
    
    // Initialise GLEW - Set GLEW to use modern techniques (glewExperimental)
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW initialisation failed.");
    }
    
    // Register input event handler
    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);
    
}

Glfw::~Glfw()
{
    glfwDestroyWindow(glfw_window);
    glfwTerminate();
}

GLFWwindow* Glfw::window() const
{
    return glfw_window;
}

inline float Glfw::aspect_ratio() {
    return (float)_width/_height;
}

inline int Glfw::width() {
    return _width;
}

inline int Glfw::height() {
    return _height;
}

bool Glfw::is_mouse_down = false;

inline bool Glfw::mouse_down() const {
    return is_mouse_down;
}

void Glfw::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Glfw::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    is_mouse_down = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
}

void Glfw::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(is_mouse_down) {
        
    }
}
