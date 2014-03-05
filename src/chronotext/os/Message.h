/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Message.html
 */

#pragma once

#include <string>

namespace chronotext
{
    struct Message
    {
        int what;
        std::string body;
        
        Message(int what)
        :
        what(what)
        {}
        
        Message(int what, const std::string &body)
        :
        what(what),
        body(body)
        {}
    };
}

namespace chr = chronotext;
