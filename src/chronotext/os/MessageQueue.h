/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY http://developer.android.com/reference/android/os/MessageQueue.html
 */

#pragma once

#include "chronotext/os/Message.h"

#include <boost/thread/mutex.hpp>

#include <queue>

namespace chronotext
{
    class MessageQueue
    {
    public:
        Message nextMessage();
        bool enqueueMessage(const Message &message);
        
    protected:
        std::queue<Message> queue;
        boost::mutex mutex;
    };
}

namespace chr = chronotext;
