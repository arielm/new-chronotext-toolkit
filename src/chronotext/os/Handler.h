/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Handler.html
 */

#pragma once

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
