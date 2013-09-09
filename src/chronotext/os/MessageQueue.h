/*
 * INSPIRED BY http://developer.android.com/reference/android/os/MessageQueue.html
 */

#pragma once

#include "chronotext/os/Message.h"

#include "cinder/Thread.h"

#include <queue>

class MessageQueue
{
    std::queue<Message> queue;
    std::mutex queueMutex;
    
public:
    Message nextMessage();
    bool enqueueMessage(const Message &message);
};
