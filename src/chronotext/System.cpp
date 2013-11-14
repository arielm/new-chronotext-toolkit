/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/System.h"

#include "cinder/System.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    /*
     * TODO: ADD ANDROID SUPPORT
     */
    string System::getIPAddress(bool maskForBroadcast)
    {
        string host = ci::System::getIpAddress();

        if (maskForBroadcast)
        {
            if (host.rfind('.') != string::npos)
            {
                host.replace(host.rfind('.') + 1, 3, "255");
            }
        }
        
        return host;
    }
}
