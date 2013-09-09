/*
 * INSPIRED BY android.os.Message
 */

#pragma once

#include "cinder/Cinder.h"

class Handler;

class Message
{
public:
    Handler *target;
    
    int what;
    int arg;
    std::shared_ptr<void> object;
    
    Message():
    target(NULL)
    {}
    
    Message(int what, int arg = 0)
    :
    what(what),
    arg(arg),
    target(NULL)
    {}
    
    Message(int what, std::shared_ptr<void> object, int arg = 0)
    :
    what(what),
    object(object),
    arg(arg),
    target(NULL)
    {}
};
