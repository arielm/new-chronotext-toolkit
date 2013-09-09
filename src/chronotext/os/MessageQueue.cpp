#include "chronotext/os/MessageQueue.h"
#include "chronotext/os/Message.h"
#include "chronotext/os/Handler.h"

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
