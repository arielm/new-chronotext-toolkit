/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Looper.html
 */

#pragma once

#include "chronotext/os/MessageQueue.h"

class Looper
{
    MessageQueue messageQueue;

public:
    void run();
    
    friend class Handler;
};
