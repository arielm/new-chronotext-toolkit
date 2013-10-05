/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

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
