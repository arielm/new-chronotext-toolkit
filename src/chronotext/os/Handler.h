/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Handler.html
 */

#pragma once

#include "chronotext/os/Message.h"

#include <boost/asio.hpp>

namespace chronotext
{
    class Handler
    {
    public:
        Handler() : io(NULL) {}
        virtual ~Handler() {}
        
        void setIOService(boost::asio::io_service &io);
        
        template <typename F> bool post(const F &fn)
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
        
        bool sendMessage(const Message &message);
        virtual void handleMessage(const Message &message) {}
        
    protected:
        boost::asio::io_service *io;
    };
}

namespace chr = chronotext;
