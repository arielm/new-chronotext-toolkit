/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SuperHandler.h"

namespace chronotext
{
    SuperHandler::SuperHandler()
    :
    Handler()
    {}
    
    void SuperHandler::addSubHandler(SubHandler *handler)
    {
        handlers.push_back(handler);
    }
    
    void SuperHandler::removeSubHandler(SubHandler *handler)
    {
        handlers.remove(handler);
    }
    
    void SuperHandler::handleMessage(const Message &message)
    {
        for (auto &handler : handlers)
        {
            if (handler->handleSubMessage(message))
            {
                break;
            }
        }
    }
}
