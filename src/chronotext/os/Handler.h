/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * INSPIRED BY http://developer.android.com/reference/android/os/Handler.html
 */

/*
 * TODO:
 *
 * 1) io_service SHOULD BE STORED IN A PostBase CLASS EXTENDED BY Handler, TaskManager, ETC.
 *
 * 2) CinderSketch IMPLEMENTATIONS SHOULD NOT STORE io_service AS WELL (I.E. SINCE THEY INHERIT FROM Handler)
 */

#pragma once

#include "chronotext/os/Message.h"

#include <boost/asio.hpp>

namespace chronotext
{
    class Handler
    {
    public:
        Handler();
        virtual ~Handler() {}

        template <typename F>
        inline bool post(const F &fn)
        {
            if (io)
            {
                io->post(fn);
                return true; // XXX
            }
            
            return false; // XXX
        }
        
        void setIOService(boost::asio::io_service &io); // XXX
        bool sendMessage(const Message &message);

        virtual void handleMessage(const Message &message) {}

    protected:
        boost::asio::io_service *io; // XXX
    };
}
