//
//  base_buffer.cpp
//  Draw App
//
//  Created by TA on 25/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#include "base_buffer.h"

/* ------------------------------------------------------------------------------------ */

BaseBuffer::BaseBuffer(const size_t b_size, const GLuint d_size, const GLuint d_stride)
: _vao{0}, _vbo{0}, flags{0}, lock{true}, index{0}, fence{nullptr}
{
    _size = b_size;
    _data_size = d_size;
    _stride = d_stride ? d_stride : sizeof(GLfloat)*d_size;
    _bytes = sizeof(GLfloat)*_size;
    flags = GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
    
    // VAO
    glGenVertexArrays(1, &_vao);
    
    // VBO
    glGenBuffers(1, &_vbo);
    
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    
    glBufferData(GL_ARRAY_BUFFER, _bytes, nullptr, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, _data_size, GL_FLOAT, GL_FALSE, _stride, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

/* ------------------------------------------------------------------------------------ */

BaseBuffer::~BaseBuffer()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

/* ------------------------------------------------------------------------------------ */
