/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED  BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Handler.html
 */

#pragma once

#include "chronotext/os/Message.h"

namespace chr
{
    class Handler
    {
    public:
        virtual ~Handler() {}

        /*
         * RETURNS FALSE IF THE MESSAGE CAN'T BE SENT
         *
         * E.G. IF THE CONTEXT IS BEING SHUT-DOWN
         *
         * REQUIREMENT SHARED WITH os/TaskManager
         */
        bool sendMessage(const Message &message);

        virtual void handleMessage(const Message &message) {}
    };
}
