//
//  main.cpp
//  Draw App
//
//  Created by TA on 22/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

/* ------------------------------------------------------------------------------------ */

#include <iostream>
#include <vector>
#include <cmath>

#include "glfw.h"
#include "shader.h"
#include "draw_buffer.h"
#include "multi_draw_buffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* ------------------------------------------------------------------------------------ */

int main(int argc, const char * argv[]) {

    // Initialise GLFW
    Glfw glfw{};
    GLFWwindow* window = glfw.window();

    // Clear colour buffer
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    
    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Dynamic Vertex Buffer
    DrawBuffer buffer{};
    
    // Shader programs
    const GLchar* vs_draw = {
        "#version 330 core\n"
        
        "layout (location = 0) in vec3 vertex;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "out vec4 colour;"
        
        "void main() {"
            "gl_Position = projection * view * vec4(vertex, 1.0);"
            "gl_PointSize = 10.0;"
            
            "if(gl_VertexID == 0) {"
                "colour = vec4(0.0, 1.0, 0.0, 1.0);"
            "} else {"
               "colour = vec4(1.0, 0.0, 0.0, 1.0);"
            "}"
        "}"
    };
    const GLchar* fs_draw = {
        "#version 330 core\n"
        
        "in vec4 colour;"
        "out vec4 outColour;"
        
        "void main() {"
            "outColour = colour;"
        "}"
    };
    Shader shader{vs_draw, fs_draw};
    
    // Transformations
    shader.use();
    glm::mat4 view;
    glm::mat4 projection;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//    projection = glm::perspective(45.0f, glfw.aspect_ratio(), 0.1f, 100.0f);
    projection = glm::ortho(0.0f, (float)glfw.width(), (float)glfw.height(), 0.0f, 0.1f, 100.0f);
    GLint view_loc = glGetUniformLocation(shader.program(), "view");
    GLint projection_loc = glGetUniformLocation(shader.program(), "projection");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    
    // Render Loop
    bool in_segment = true;
    bool write_successful = false;
    double prev_xpos = 0.0, prev_ypos = 0.0;
    while(!glfwWindowShouldClose(window)) {

        // Resize if necessary
        buffer.resize();
        
        // Check for and execute events - block if no events.
        glfwWaitEvents();
        
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the buffer
        if(glfw.mouse_down()) {
            
            // Get cursor position
            double xpos, ypos;
            glfwGetCursorPos(glfw.window(), &xpos, &ypos);
            
            // Write new positions to buffer
            if(prev_xpos != xpos || prev_ypos != ypos || !in_segment) {
                
                // Start a new line segment
                if(!in_segment) {
                    in_segment = true;
                    buffer.start_segment();
                }
                
                prev_xpos = xpos;
                prev_ypos = ypos;
                buffer.write_to_buffer(xpos, ypos);
                
            }
        } else if(in_segment) {
            in_segment = false;
            buffer.end_segment();
        }
        
        // Draw
        shader.use();
        buffer.draw();
            
        // Swap the buffer
        glfwSwapBuffers(window);

    }
}

/* ------------------------------------------------------------------------------------ */
