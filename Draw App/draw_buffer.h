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

class DrawBuffer : public BaseBuffer
{
public:
    using BaseBuffer::BaseBuffer;
    DrawBuffer();
    ~DrawBuffer();
    void start_segment() override;
    void end_segment() override;
    bool write_to_buffer(float xpos, float ypos) override;
    void draw() override;
    void resize();
    
private:
    std::vector<Segment> segments;
};

/* ------------------------------------------------------------------------------------ */

#endif
