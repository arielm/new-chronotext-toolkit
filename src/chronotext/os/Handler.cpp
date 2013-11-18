/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/os/Handler.h"

using namespace std;

namespace chronotext
{
    void Handler::setIOService(boost::asio::io_service &io)
    {
        this->io = &io;
    }
    
    bool Handler::post(const function<void()> &fn)
    {
        if (io)
        {
            io->post(fn);
            return true;
        }
        else
        {
            return false;
        }
    }
    
    bool Handler::sendMessage(const Message &message)
    {
        if (io)
        {
            io->post(bind(&Handler::handleMessage, this, message));
            return true;
        }
        
        return false;
    }
}
