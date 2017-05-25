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
#include "glfw.h"
#include "shader.h"
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

template<class T, GLenum U>
class BaseBuffer
{
public:
    BaseBuffer(const size_t, const GLuint, const GLuint = 0);
    ~BaseBuffer();
    GLuint vao() const;
    GLuint vbo() const;
    size_t size() const;
    size_t bytes() const;
    GLuint stride() const;
    GLuint data_size() const;
    virtual void render(const Glfw&, const Shader&) = 0;

protected:
    GLuint _vao;
    GLuint _vbo;
    size_t _size;
    size_t _bytes;
    GLuint _stride;
    GLuint _data_size;
    GLuint index;
    bool in_segment;
    GLsync fence;
    GLbitfield flags;
    BufferRangeLock lock;
    double prev_xpos, prev_ypos;
    void configure_buffer(GLenum target);
};

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
BaseBuffer<T, U>::BaseBuffer(const size_t b_size, const GLuint d_size, const GLuint d_stride)
: _vao{0}, _vbo{0}, _size{b_size}, _data_size{d_size}, lock{true}, in_segment{false}, index{0}, fence{nullptr}, prev_xpos{0}, prev_ypos{0}
{
    // VAO
    glGenVertexArrays(1, &_vao);
    
    // VBO
    glGenBuffers(1, &_vbo);
    
    // Vertex Buffer
    _stride = d_stride;
    _bytes = sizeof(T)*_size;
    flags = GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
void BaseBuffer<T, U>::configure_buffer(GLenum target)
{
    glBindVertexArray(_vao);
    glBindBuffer(target, _vbo);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, _data_size, U, GL_FALSE, _stride, (GLvoid*)0);
    glBufferData(target, _bytes, nullptr, GL_DYNAMIC_DRAW);
    
    glBindBuffer(target, 0);
    glBindVertexArray(0);
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
BaseBuffer<T, U>::~BaseBuffer()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
inline GLuint BaseBuffer<T, U>::vao() const
{
    return _vao;
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
inline GLuint BaseBuffer<T, U>::vbo() const
{
    return _vbo;
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
inline GLuint BaseBuffer<T, U>::data_size() const
{
    return _data_size;
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
inline GLuint BaseBuffer<T, U>::stride() const
{
    return _stride;
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
inline size_t BaseBuffer<T, U>::size() const
{
    return _size;
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
inline size_t BaseBuffer<T, U>::bytes() const
{
    return _bytes;
}

/* ------------------------------------------------------------------------------------ */

#endif
