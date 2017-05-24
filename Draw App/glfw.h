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

/* ------------------------------------------------------------------------------------ */

class Glfw {
public:
    Glfw();
    ~Glfw();
    int width() const;
    int height() const;
    float aspect_ratio() const;
    float normalise_xpos(float) const;
    float normalise_ypos(float) const;
    bool mouse_down() const;
    GLFWwindow* window() const;
private:
    int _width, _height;
    float _half_width, _half_height;
    GLFWwindow* glfw_window;
    static bool is_mouse_down;
    static void key_callback(GLFWwindow*, int, int, int, int);
    static void mouse_button_callback(GLFWwindow*, int, int, int);
    static void cursor_position_callback(GLFWwindow*, double, double);
};

/* ------------------------------------------------------------------------------------ */

inline GLFWwindow* Glfw::window() const
{
    return glfw_window;
}

/* ------------------------------------------------------------------------------------ */

inline int Glfw::width() const
{
    return _width;
}

/* ------------------------------------------------------------------------------------ */

inline int Glfw::height() const
{
    return _height;
}

/* ------------------------------------------------------------------------------------ */

inline float Glfw::aspect_ratio() const
{
    return (float)_width/_height;
}

/* ------------------------------------------------------------------------------------ */

inline float Glfw::normalise_xpos(float xpos) const
{
    return (xpos / _half_width) - 1;
}

/* ------------------------------------------------------------------------------------ */

inline float Glfw::normalise_ypos(float ypos) const
{
    return 1 - (ypos / _half_height);
}

/* ------------------------------------------------------------------------------------ */

inline bool Glfw::mouse_down() const
{
    return is_mouse_down;
}

/* ------------------------------------------------------------------------------------ */

#endif
