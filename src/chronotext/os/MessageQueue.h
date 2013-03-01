/*
 * INSPIRED BY android.os.MessageQueue
 */

#pragma once

#include "chronotext/os/Message.h"

#include <queue>

class MessageQueue
{
    std::queue<Message> queue;
    std::mutex queueMutex;
    
public:
    Message nextMessage();
    bool enqueueMessage(const Message &message);
};
