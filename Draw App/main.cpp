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
    
    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    
    // VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);
    
    // Dynamic Vertex Buffer
    GLuint length = 3;
    GLuint stride = sizeof(GLfloat)*length;
    size_t buffer_size = std::pow(2.0, 3.0);
    size_t buffer_bytes = sizeof(GLfloat)*buffer_size;
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, length, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glBufferData(GL_ARRAY_BUFFER, buffer_bytes, nullptr, GL_DYNAMIC_DRAW);
//    GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//    glBufferStorage(GL_ARRAY_BUFFER, buffer_bytes, nullptr, flags | GL_DYNAMIC_STORAGE_BIT);
//    GLvoid* mapped_data = glMapBufferRange(GL_ARRAY_BUFFER, 0, buffer_size, flags);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
//    if(mapped_data == nullptr) { throw std::runtime_error("Failed to map buffer."); }
//    GLfloat* data = static_cast<GLfloat*>(mapped_data);
    
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

    // Render loop
    GLuint index = 0;
    bool start = true;
    double prev_xpos, prev_ypos;
    GLsync fence = nullptr;
    BufferRangeLock lock{};
    GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
    while(!glfwWindowShouldClose(window)) {

        if(buffer_size <= (index+1)*stride) {
        
            // NOTE: Use sparse buffers if available.
            
            buffer_size *= 2;
            buffer_bytes = sizeof(GLfloat)*buffer_size;
            
            GLuint tmp_vbo;
            glGenBuffers(1, &tmp_vbo);
            
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, tmp_vbo);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, length, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            glBufferData(GL_ARRAY_BUFFER, buffer_bytes, nullptr, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            
            glBindBuffer(GL_COPY_READ_BUFFER, vbo);
            glBindBuffer(GL_COPY_WRITE_BUFFER, tmp_vbo);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, index*stride);
            
            glDeleteBuffers(1, &vbo);
            vbo = tmp_vbo;
            
        }
        
        // Check for and execute events - block if no events.
        glfwWaitEvents();
        
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Cursor position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        
        GLuint start_index = index*stride;
        bool write = false;
        
        // Write to the buffer
        if(prev_xpos != xpos || prev_ypos != ypos || start) {
            prev_xpos = xpos;
            prev_ypos = ypos;
            start = false;
            
            lock.remove_signaled_locks(0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            GLfloat* data = (GLfloat*)glMapBufferRange(GL_ARRAY_BUFFER, start_index, length, flags);
            
            if(data != nullptr) {
                *data++ = xpos; //glfw.normalise_xpos(xpos);
                *data++ = ypos; //glfw.normalise_ypos(ypos);
                *data = 0.0;
                glFlushMappedBufferRange(GL_ARRAY_BUFFER, start_index, length);
                
            }
            
            if(glUnmapBuffer(GL_ARRAY_BUFFER)) {
                ++index;
                write = true;
            } else {
                std::cout << "Map Failed.\n";
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
        }
        
        // Draw
        shader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_LINE_STRIP, 0, index);
        if(write) { lock.lock_range(start_index, length); }
        fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glBindVertexArray(0);
        
        // Swap the buffer
        glfwSwapBuffers(window);
    }
}

/* ------------------------------------------------------------------------------------ */
