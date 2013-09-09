/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Looper.html
 */

#pragma once

#include "chronotext/os/MessageQueue.h"

class Looper
{
public:
    MessageQueue messageQueue;

    void run();
};
