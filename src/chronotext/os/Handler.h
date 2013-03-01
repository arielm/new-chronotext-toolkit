/*
 * INSPIRED BY android.os.Handler
 */

#pragma once

#include "chronotext/os/Message.h"
#include "chronotext/os/MessageQueue.h"
#include "chronotext/os/Looper.h"

class Handler
{
    Looper *looper;
    
public:
    Handler(Looper *looper) : looper(looper) {}
    virtual ~Handler() {}
    
    bool sendMessage(const Message &message);
    virtual void handleMessage(const Message &message) {}
};
