//
//  draw_buffer.cpp
//  Draw App
//
//  Created by TA on 25/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#include "draw_buffer.h"
#include <GLFW/glfw3.h>

/* ------------------------------------------------------------------------------------ */

DrawBuffer::DrawBuffer()
: BaseBuffer{static_cast<size_t>(std::pow(2.0, 20.0)), 4}
{
    
}

/* ------------------------------------------------------------------------------------ */

DrawBuffer::~DrawBuffer()
{
    
}

/* ------------------------------------------------------------------------------------ */

void DrawBuffer::start_segment()
{
    segments.push_back(Segment{index, 0, glfwGetTime()});
}
/* ------------------------------------------------------------------------------------ */

void DrawBuffer::end_segment()
{
    if(segments.size() != 0 && segments.back().count == 0) {
        segments.pop_back();
    }
}

/* ------------------------------------------------------------------------------------ */

bool DrawBuffer::write_to_buffer(float xpos, float ypos)
{
    
    
    lock.remove_signaled_locks(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    GLuint start_index = index * _stride;
    GLfloat* data = (GLfloat*)glMapBufferRange(GL_ARRAY_BUFFER, start_index, _data_size, flags);
    
    if(data != nullptr) {
        *data++ = xpos;
        *data++ = ypos;
        *data++ = 0.0; // z
        *data = 1.0; // alpha
        
        glFlushMappedBufferRange(GL_ARRAY_BUFFER, start_index, _data_size);
    }
    
    bool write_successful = glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if(write_successful) {
        ++index;
        ++segments.back();
        times.push_back(glfwGetTime());
    }
    
    return write_successful;
}

/* ------------------------------------------------------------------------------------ */

void DrawBuffer::update_buffer()
{

}

/* ------------------------------------------------------------------------------------ */

void DrawBuffer::draw()
{
    glBindVertexArray(_vao);
    for(auto& segment : segments) {
        glDrawArrays(GL_LINE_STRIP, segment.index, segment.count);
    }
    glBindVertexArray(0);
}

/* ------------------------------------------------------------------------------------ */

void DrawBuffer::resize()
{
    if(_size <= (index+1)*_stride) {
        _size *= 2;
        _bytes = sizeof(GLfloat)*_size;
        
        GLuint tmp_vbo;
        glGenBuffers(1, &tmp_vbo);

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, tmp_vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, _data_size, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glBufferData(GL_ARRAY_BUFFER, _bytes, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindBuffer(GL_COPY_READ_BUFFER, _vbo);
        glBindBuffer(GL_COPY_WRITE_BUFFER, tmp_vbo);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, index*_stride);

        glDeleteBuffers(1, &_vbo);
        _vbo = tmp_vbo;
    }
}

/* ------------------------------------------------------------------------------------ */
