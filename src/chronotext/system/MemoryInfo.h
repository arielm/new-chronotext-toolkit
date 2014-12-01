/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

namespace chronotext
{
    struct MemoryInfo
    {
        uint64_t available;
        uint64_t total;
        uint64_t used;
        int warningLevel;
        
        MemoryInfo()
        :
        available(0),
        total(0),
        used(0),
        warningLevel(0)
        {}
    };
}

namespace chr = chronotext;
