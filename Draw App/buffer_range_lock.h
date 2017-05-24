//
//  buffer_range_lock.h
//  Draw App
//
//  Created by TA on 24/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//
// Source: https://github.com/nvMcJohn/apitest/blob/master/src/framework/bufferlock.h
//

#ifndef buffer_range_lock_h
#define buffer_range_lock_h

#include <iostream>
#include <list>
#include <GL/glew.h>

/* ------------------------------------------------------------------------------------ */

struct BufferRange
{
    size_t index;
    size_t length;
    
    bool overlaps(const BufferRange& range) const {
        return index < (range.index + range.length) && range.index < (index + length);
    }
};

/* ------------------------------------------------------------------------------------ */

struct BufferLock
{
    BufferRange range;
    GLsync fence;
    bool signaled();
};

/* ------------------------------------------------------------------------------------ */

class BufferRangeLock
{
public:
    BufferRangeLock(bool _cpu_updates = true);
    ~BufferRangeLock();
    void remove_signaled_locks(int count);
    void lock_range(size_t index, size_t length);
    void wait_for_locked_range(size_t index, size_t length);
    
private:
    bool cpu_updates;
    void wait(const GLsync& fence);
    void remove(const BufferLock& lock) const;
    std::list<BufferLock> locks;
};

/* ------------------------------------------------------------------------------------ */

#endif
