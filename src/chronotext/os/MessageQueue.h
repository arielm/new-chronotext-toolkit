/*
 * INSPIRED BY android.os.MessageQueue
 */

#pragma once

#include "chronotext/os/Message.h"

#include <boost/thread/mutex.hpp>

#include <queue>

class MessageQueue
{
    std::queue<Message> queue;
    boost::mutex queueMutex;
    
public:
    Message nextMessage();
    bool enqueueMessage(const Message &message);
};
