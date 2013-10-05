/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/MessageQueue.h"

Message MessageQueue::nextMessage()
{
    Message message;
    
#ifndef MESSAGE_QUEUE_LOCK_FREE
    boost::mutex::scoped_lock lock(mutex);
#endif
    
    if (!queue.empty())
    {
        message = queue.front();
        queue.pop();
    }

    return message;
}

bool MessageQueue::enqueueMessage(const Message &message)
{
#ifndef MESSAGE_QUEUE_LOCK_FREE
    boost::mutex::scoped_lock lock(mutex);
#endif
    
    queue.push(message);
    return true;
}
