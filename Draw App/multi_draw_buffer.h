////
////  multi_draw_buffer.h
////  Draw App
////
////  Created by TA on 25/05/2017.
////  Copyright © 2017 TA. All rights reserved.
////
//
//#ifndef multi_draw_buffer_h
//#define multi_draw_buffer_h
//
//#include "base_buffer.h"
//#include <vector>
//
///* ------------------------------------------------------------------------------------ */
//
//typedef struct {
//    unsigned int  count;
//    unsigned int  instanceCount;
//    unsigned int  first;
//    unsigned int  baseInstance;
//} DrawArraysIndirectCommand;
//
///* ------------------------------------------------------------------------------------ */
//
//template<class T, GLenum U>
//class MultiDrawBuffer : public BaseBuffer<T, U>
//{
//public:
//    using BaseBuffer<T, U>::BaseBuffer;
//    MultiDrawBuffer();
//    ~MultiDrawBuffer();
//    virtual void render(const Glfw&, const Shader&) override;
//        
//private:
//    std::vector<DrawArraysIndirectCommand> commands;
//};
//
///* ------------------------------------------------------------------------------------ */
//
//template<class T, GLenum U>
//MultiDrawBuffer<T, U>::MultiDrawBuffer()
//: BaseBuffer<T, U>{static_cast<size_t>(std::pow(2.0, 20.0)), 3}
//{
//    this->configure_buffer(GL_DRAW_INDIRECT_BUFFER);
//}
//
///* ------------------------------------------------------------------------------------ */
//
//template<class T, GLenum U>
//MultiDrawBuffer<T, U>::~MultiDrawBuffer()
//{
//    
//}
//
///* ------------------------------------------------------------------------------------ */
//
//template<class T, GLenum U>
//void MultiDrawBuffer<T, U>::render(const Glfw& glfw, const Shader& shader)
//{
//    bool write_successful = false;
//    GLuint start_index = this->index*this->_stride;
//    
//    if(glfw.mouse_down()) {
//        
//        // Cursor position
//        double xpos, ypos;
//        glfwGetCursorPos(glfw.window(), &xpos, &ypos);
//        
//        // Write to the buffer
//        if(this->prev_xpos != xpos || this->prev_ypos != ypos || !(this->in_segment)) {
//            
//            if(!this->in_segment) {
//                commands.push_back(DrawArraysIndirectCommand{0, 1, (unsigned)this->index, 0});
//            }
//            
//            this->in_segment = true;
//            this->prev_xpos = xpos;
//            this->prev_ypos = ypos;
//            
//            this->lock.remove_signaled_locks(0);
//            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, this->_vbo);
//            GLfloat* data = (GLfloat*)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, start_index, this->_data_size, this->flags);
//            
//            if(data != nullptr) {
//                *data++ = xpos; //glfw.normalise_xpos(xpos);
//                *data++ = ypos; //glfw.normalise_ypos(ypos);
//                *data = 0.0;
//                glFlushMappedBufferRange(GL_DRAW_INDIRECT_BUFFER, start_index, this->_data_size);
//                
//            }
//            
//            if(glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER)) {
//                ++this->index;
//                write_successful = true;
//                commands.back().count = commands.back().count + 1;
//            } else {
//                std::cout << "Map Failed.\n";
//            }
//            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
//            
//        }
//    } else if(!this->in_segment) {
//        this->in_segment = true;
//    }
//    
//    if(write_successful) {
//        // Draw
//        shader.use();
//        glBindVertexArray(this->_vao);
//        glMultiDrawArraysIndirect(GL_LINE_STRIP, &*commands.begin(), commands.size(), 0);
//        this->lock.lock_range(start_index, this->_data_size);
//        this->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
//        glBindVertexArray(0);
//        
//        // Swap the buffer
//        glfwSwapBuffers(glfw.window());
//    }
//}
//
///* ------------------------------------------------------------------------------------ */
//
//#endif
