/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <cstdint>

namespace chronotext
{
    namespace memory
    {
        struct Info
        {
            int64_t free;
            int64_t used;
            double ratio;
            
            Info(int64_t free = -1, int64_t used = -1, double ratio = 0)
            :
            free(free),
            used(used),
            ratio(ratio)
            {}
        };
    }
}

namespace chr = chronotext;
