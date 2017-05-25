//
//  draw_buffer.hpp
//  Draw App
//
//  Created by TA on 25/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#ifndef draw_buffer_h
#define draw_buffer_h

#include "base_buffer.h"

/* ------------------------------------------------------------------------------------ */

struct Segment
{
    GLuint index;
    GLuint count;
    GLuint& operator ++()
    {
        ++count;
        return count;
    };
};

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
class DrawBuffer : public BaseBuffer<T, U>
{
public:
    using BaseBuffer<T, U>::BaseBuffer;
    DrawBuffer();
    ~DrawBuffer();
    void render(const Glfw&, const Shader&);
    
private:
    std::vector<Segment> segments;
};

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
DrawBuffer<T, U>::DrawBuffer()
: BaseBuffer<T, U>{static_cast<size_t>(std::pow(2.0, 20.0)), 3}
{
    this->configure_buffer(GL_ARRAY_BUFFER);
}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
DrawBuffer<T, U>::~DrawBuffer()
{

}

/* ------------------------------------------------------------------------------------ */

template<class T, GLenum U>
void DrawBuffer<T, U>::render(const Glfw& glfw, const Shader& shader)
{
    bool write_successful = false;
    GLuint start_index = this->index*this->_stride;
    
    if(glfw.mouse_down()) {
        
        // Cursor position
        double xpos, ypos;
        glfwGetCursorPos(glfw.window(), &xpos, &ypos);
        
        // Write to the buffer
        if(this->prev_xpos != xpos || this->prev_ypos != ypos || !(this->in_segment)) {
            
            if(!this->in_segment && this->index == segments.size()) {
                segments.push_back({this->index, 0});
            }
            
            this->in_segment = true;
            this->prev_xpos = xpos;
            this->prev_ypos = ypos;
            
            this->lock.remove_signaled_locks(0);
            glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
            T* data = (T*)glMapBufferRange(GL_ARRAY_BUFFER, start_index, this->_data_size, this->flags);
            
            if(data != nullptr) {
                *data++ = xpos; //glfw.normalise_xpos(xpos);
                *data++ = ypos; //glfw.normalise_ypos(ypos);
                *data = 0.0;
                glFlushMappedBufferRange(GL_ARRAY_BUFFER, start_index, this->_data_size);
            }
            
            if(glUnmapBuffer(GL_ARRAY_BUFFER)) {
                ++this->index;
                write_successful = true;
                ++segments.back();
            } else {
                std::cout << "Map Failed.\n";
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
        }
    } else if(this->in_segment) {
        this->in_segment = false;
    }
    
    if(write_successful) {
        
        std::cout << this->index << std::endl;
    
        // Draw
        shader.use();
        glBindVertexArray(this->_vao);
        glDrawArrays(GL_LINE_STRIP, 0, this->index);
//        for(auto& segment : segments) {
//            glDrawArrays(GL_LINE_STRIP, segment.index, segment.count);
//        }
        
        this->lock.lock_range(start_index, this->_data_size);
        this->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glBindVertexArray(0);
        
        // Swap the buffer
        glfwSwapBuffers(glfw.window());
    }
}

/* ------------------------------------------------------------------------------------ */

#endif
