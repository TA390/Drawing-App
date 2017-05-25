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
#include "buffer_range_lock.h"
#include "multi_draw_buffer.h"
#include "draw_buffer.h"
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

    
    if(glewIsSupported("ARB_multi_draw_indirect")) {
        std::cout << "Supported!!!\n";
    }
    
    // Clear colour buffer
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    
    // Configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Dynamic Vertex Buffer
    DrawBuffer<GLfloat, GL_FLOAT> buffer{};
    
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
    GLint viewLoc = glGetUniformLocation(shader.program(), "view");
    GLint projLoc = glGetUniformLocation(shader.program(), "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    
    while(!glfwWindowShouldClose(window)) {

//        if(buffer.size() <= (index+1)*buffer.stride()) {
//        
//            // NOTE: Use sparse buffers if available.
//            
//            buffer_size *= 2;
//            buffer_bytes = sizeof(GLfloat)*buffer_size;
//            
//            GLuint tmp_vbo;
//            glGenBuffers(1, &tmp_vbo);
//            
//            glBindVertexArray(vao);
//            glBindBuffer(GL_ARRAY_BUFFER, tmp_vbo);
//            glEnableVertexAttribArray(0);
//            glVertexAttribPointer(0, length, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//            glBufferData(GL_ARRAY_BUFFER, buffer_bytes, nullptr, GL_DYNAMIC_DRAW);
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
//            glBindVertexArray(0);
//            
//            glBindBuffer(GL_COPY_READ_BUFFER, vbo);
//            glBindBuffer(GL_COPY_WRITE_BUFFER, tmp_vbo);
//            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, index*stride);
//            
//            glDeleteBuffers(1, &vbo);
//            vbo = tmp_vbo;
//            
//        }
        
        // Check for and execute events - block if no events.
        glfwWaitEvents();
        
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render
        buffer.render(glfw, shader);

    }
}

/* ------------------------------------------------------------------------------------ */
