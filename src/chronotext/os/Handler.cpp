/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/Handler.h"

namespace chronotext
{
    bool Handler::sendMessage(const Message &message)
    {
        if (looper)
        {
            Message tmp = message;
            tmp.target = this;
            
            return looper->messageQueue.enqueueMessage(tmp);
        }
        
        return false;
    }
}
