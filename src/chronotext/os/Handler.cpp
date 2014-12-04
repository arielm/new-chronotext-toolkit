/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Handler.h"

#include "chronotext/system/Context.h"

using namespace std;

namespace chronotext
{
    /*
     * TODO: SHOULD RETURN FALSE IF THE MESSAGE CAN'T BE SENT
     *
     * E.G. THE CONTEXT IS BEING SHUT-DOWN...
     */

    bool Handler::sendMessage(const Message &message)
    {
        /*
         * TODO:
         *
         * OPTION 1: CONSIDER USING C++11 LAMBDA INSTEAD OF bind
         *
         * OPTION 2: return context::post([=]{ handleMessage(message); });
         */
        
        context::io().post(bind(&Handler::handleMessage, this, message));
        
        return true; // XXX
    }
}
