//
//  base_buffer.h
//  Draw App
//
//  Created by TA on 25/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#ifndef base_buffer_h
#define base_buffer_h

#include <stdio.h>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include "buffer_range_lock.h"

//    GLuint length = 3;
//    GLuint stride = sizeof(GLfloat)*length;
//    size_t buffer_size = std::pow(2.0, 3.0);
//    size_t buffer_bytes = sizeof(GLfloat)*buffer_size;
//    glBindVertexArray(vao);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, length, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//    glBufferData(GL_ARRAY_BUFFER, buffer_bytes, nullptr, GL_DYNAMIC_DRAW);
////    GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
////    glBufferStorage(GL_ARRAY_BUFFER, buffer_bytes, nullptr, flags | GL_DYNAMIC_STORAGE_BIT);
////    GLvoid* mapped_data = glMapBufferRange(GL_ARRAY_BUFFER, 0, buffer_size, flags);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
////    if(mapped_data == nullptr) { throw std::runtime_error("Failed to map buffer."); }
////    GLfloat* data = static_cast<GLfloat*>(mapped_data);


/* ------------------------------------------------------------------------------------ */

class BaseBuffer
{
public:
    BaseBuffer(const size_t, const GLuint, const GLuint = 0);
    virtual ~BaseBuffer();
    GLuint vao() const;
    GLuint vbo() const;
    size_t size() const;
    size_t bytes() const;
    GLuint stride() const;
    GLuint data_size() const;
    virtual void start_segment() = 0;
    virtual void end_segment() = 0;
    virtual bool write_to_buffer(float xpos, float ypos) = 0;
    virtual void draw() = 0;
    
protected:
    GLuint _vao;
    GLuint _vbo;
    size_t _size;
    size_t _bytes;
    GLuint _stride;
    GLuint _data_size;
    GLuint index;
    GLsync fence;
    GLbitfield flags;
    BufferRangeLock lock;
};

/* ------------------------------------------------------------------------------------ */

inline GLuint BaseBuffer::vao() const
{
    return _vao;
}

/* ------------------------------------------------------------------------------------ */

inline GLuint BaseBuffer::vbo() const
{
    return _vbo;
}

/* ------------------------------------------------------------------------------------ */

inline GLuint BaseBuffer::data_size() const
{
    return _data_size;
}

/* ------------------------------------------------------------------------------------ */

inline GLuint BaseBuffer::stride() const
{
    return _stride;
}

/* ------------------------------------------------------------------------------------ */

inline size_t BaseBuffer::size() const
{
    return _size;
}

/* ------------------------------------------------------------------------------------ */

inline size_t BaseBuffer::bytes() const
{
    return _bytes;
}

/* ------------------------------------------------------------------------------------ */

#endif
