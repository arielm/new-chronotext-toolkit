/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/Handler.h"
#include "chronotext/os/SubHandler.h"

#include <list>

#pragma once

namespace chronotext
{
    class SuperHandler : public Handler
    {
    public:
        void addSubHandler(SubHandler *handler);
        void removeSubHandler(SubHandler *handler);
        
        void handleMessage(const Message &message) override;
        
    protected:
        std::list<SubHandler*> handlers;
    };
}
