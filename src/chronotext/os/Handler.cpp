/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Handler.h"

#include "chronotext/Context.h"

using namespace std;
using namespace context;

namespace chr
{
    bool Handler::sendMessage(const Message &message)
    {
        /*
         * TODO: CONSIDER USING A LAMBDA INSTEAD OF bind
         */
        return os::post(bind(&Handler::handleMessage, this, message));
    }
}
