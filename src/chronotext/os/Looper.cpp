/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/Looper.h"
#include "chronotext/os/Handler.h"

namespace chronotext
{
    void Looper::run()
    {
        while (true)
        {
            Message message = messageQueue.nextMessage();
            
            if (message.target)
            {
                message.target->handleMessage(message);
            }
            else
            {
                break;
            }
        }
    }
}
