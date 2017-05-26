//
//  multi_draw_buffer.h
//  Draw App
//
//  Created by TA on 25/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//

#ifndef multi_draw_buffer_h
#define multi_draw_buffer_h

#include "base_buffer.h"
#include <vector>

/* ------------------------------------------------------------------------------------ */

typedef struct {
    unsigned int  count;
    unsigned int  instanceCount;
    unsigned int  first;
    unsigned int  baseInstance;
} DrawArraysIndirectCommand;

/* ------------------------------------------------------------------------------------ */

class MultiDrawBuffer : public BaseBuffer
{
public:
    using BaseBuffer::BaseBuffer;
    MultiDrawBuffer();
    ~MultiDrawBuffer();
    void start_segment() override;
    void end_segment() override;
    bool write_to_buffer(float xpos, float ypos) override;
    void draw() override;
    void resize();
        
private:
    std::vector<DrawArraysIndirectCommand> commands;
};

/* ------------------------------------------------------------------------------------ */

#endif
