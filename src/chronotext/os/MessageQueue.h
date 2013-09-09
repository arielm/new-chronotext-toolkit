/*
 * INSPIRED BY http://developer.android.com/reference/android/os/MessageQueue.html
 */

#pragma once

#include "chronotext/os/Message.h"

#include <boost/thread/mutex.hpp>

#include <queue>

class MessageQueue
{
    std::queue<Message> queue;
    boost::mutex mutex;
    
public:
    Message nextMessage();
    bool enqueueMessage(const Message &message);
};
