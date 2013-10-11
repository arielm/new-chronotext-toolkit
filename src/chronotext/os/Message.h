/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Message.html
 */

#pragma once

#include <memory>
#include <string>

namespace chronotext
{
    class Handler;

    class Message
    {
    public:
        int what;
        std::shared_ptr<void> object;
        int64_t arg;
        
        Handler *target;
        
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
        
        Message(int what, const std::string &object, int64_t arg = 0)
        :
        what(what),
        object(std::make_shared<std::string>(object)),
        arg(arg),
        target(NULL)
        {}
        
        std::string& string() const
        {
            return *std::static_pointer_cast<std::string>(object);
        }
    };
}

namespace chr = chronotext;
