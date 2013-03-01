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
    std::shared_ptr<void> object;
    
    Message() : target(NULL) {}
    Message(int what) : target(NULL), what(what) {}
    Message(int what, std::shared_ptr<void> object) : target(NULL), what(what), object(object) {}
};
