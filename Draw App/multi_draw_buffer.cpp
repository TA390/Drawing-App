//
//  multi_draw_buffer.cpp
//  Draw App
//
//  Created by TA on 25/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#include "multi_draw_buffer.h"

/* ------------------------------------------------------------------------------------ */

MultiDrawBuffer::MultiDrawBuffer()
: BaseBuffer{static_cast<size_t>(std::pow(2.0, 20.0)), 3}
{
    
}

/* ------------------------------------------------------------------------------------ */

MultiDrawBuffer::~MultiDrawBuffer()
{
    
}

/* ------------------------------------------------------------------------------------ */

void MultiDrawBuffer::start_segment()
{
    commands.push_back(DrawArraysIndirectCommand{0, 1, (unsigned)index, 0});
}
/* ------------------------------------------------------------------------------------ */

void MultiDrawBuffer::end_segment()
{
    if(commands.size() != 0 && commands.back().count == 0) {
        commands.pop_back();
    }
}

/* ------------------------------------------------------------------------------------ */

bool MultiDrawBuffer::write_to_buffer(float xpos, float ypos)
{
    lock.remove_signaled_locks(0);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, _vbo);
    GLuint start_index = index * _stride;
    GLfloat* data = (GLfloat*)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, start_index, _data_size, flags);
    
    if(data != nullptr) {
        *data++ = xpos;
        *data++ = ypos;
        *data = 0.0;
        glFlushMappedBufferRange(GL_DRAW_INDIRECT_BUFFER, start_index, _data_size);
    }
    
    bool write_successful = glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    
    if(write_successful) {
        ++index;
        commands.back().count = commands.back().count + 1;
    }
    
    return write_successful;
}

/* ------------------------------------------------------------------------------------ */

void MultiDrawBuffer::draw()
{
    glBindVertexArray(_vao);
    glMultiDrawArraysIndirect(GL_LINE_STRIP, &*commands.begin(), commands.size(), 0);
    glBindVertexArray(0);
}

/* ------------------------------------------------------------------------------------ */

void MultiDrawBuffer::resize()
{

}

/* ------------------------------------------------------------------------------------ */
