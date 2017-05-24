//
//  buffer_range_lock.cpp
//  Draw App
//
//  Created by TA on 24/05/2017.
//  Copyright © 2017 TA. All rights reserved.
//
// Source: https://github.com/nvMcJohn/apitest/blob/master/src/framework/bufferlock.cpp
//

#include "buffer_range_lock.h"

/* ------------------------------------------------------------------------------------ */

bool BufferLock::signaled() {
    GLint signal;
    glGetSynciv(fence, GL_SYNC_STATUS, sizeof(GLint), nullptr, &signal);
    return signal == GL_SIGNALED;
}

/* ------------------------------------------------------------------------------------ */

BufferRangeLock::BufferRangeLock(bool _cpu_updates)
: cpu_updates(_cpu_updates)
{

}

/* ------------------------------------------------------------------------------------ */

BufferRangeLock::~BufferRangeLock()
{
    for(auto it = locks.begin(); it != locks.end(); ++it) {
        remove(*it);
    }
}

/* ------------------------------------------------------------------------------------ */

void BufferRangeLock::remove_signaled_locks(int count)
{
    auto it = locks.begin();
    const auto end = locks.end();
    for(; count > 0 && it != end && it->signaled(); ++it, --count) {
        locks.erase(it);
    }
}

/* ------------------------------------------------------------------------------------ */

void BufferRangeLock::wait_for_locked_range(size_t index, size_t length)
{
    BufferRange range{ index, length };
    
    for(auto it = locks.begin(); it != locks.end(); ++it)
    {
        if(range.overlaps(it->range)) {
            wait(it->fence);
            locks.erase(it);
        } else if(it->signaled()) {
            locks.erase(it);
        }
    }
}

/* ------------------------------------------------------------------------------------ */

void BufferRangeLock::lock_range(size_t index, size_t length)
{
    GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    BufferRange range{ index, length };
    BufferLock new_lock{ range, fence };
    locks.push_back(new_lock);
}

/* ------------------------------------------------------------------------------------ */

void BufferRangeLock::wait(const GLsync& fence)
{
    if(cpu_updates) {
        GLenum status = glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        if(status != GL_WAIT_FAILED) { std::cerr << "glClientWaitSync Failed." << std::endl; }
    }
    
    glWaitSync(fence, 0, GL_TIMEOUT_IGNORED);
}

/* ------------------------------------------------------------------------------------ */

void BufferRangeLock::remove(const BufferLock& lock) const
{
    glDeleteSync(lock.fence);
}

/* ------------------------------------------------------------------------------------ */
