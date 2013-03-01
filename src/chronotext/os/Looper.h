/*
 * INSPIRED BY android.os.Looper
 */

#pragma once

#include "chronotext/os/MessageQueue.h"

class Looper
{
public:
    MessageQueue messageQueue;

    void run();
};
