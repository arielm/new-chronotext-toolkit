/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Message.html
 */

#pragma once

#include <memory>

class Handler;

class Message
{
public:
    Handler *target;
    
    int what;
    int64_t arg;
    std::shared_ptr<void> object;
    
    Message():
    target(NULL)
    {}
    
    Message(int what, int64_t arg = 0)
    :
    what(what),
    arg(arg),
    target(NULL)
    {}
    
    Message(int what, std::shared_ptr<void> object, int64_t arg = 0)
    :
    what(what),
    object(object),
    arg(arg),
    target(NULL)
    {}
};
