/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/utils/Utils.h"

namespace chr
{
    namespace memory
    {
        struct Info
        {
            int64_t free;
            int64_t used;
            
            Info(int64_t free = -1, int64_t used = -1)
            :
            free(free),
            used(used)
            {}
            
            friend std::ostream& operator<<(std::ostream &lhs, const Info &rhs)
            {
                lhs << "{";
                
                if (rhs.free > 0)
                {
                    lhs << "free: " << utils::format::bytes(rhs.free);
                }
                
                if (rhs.used > 0)
                {
                    if (rhs.free > 0)
                    {
                        lhs << ", ";
                    }
                    
                    lhs << "used: " << utils::format::bytes(rhs.used);
                }
                
                if ((rhs.free > 0) && (rhs.used > 0))
                {
                    double ratio = rhs.used / double(rhs.free + rhs.used);
                    
                    lhs << ", percent: " << utils::format::percent(ratio);
                }
                
                lhs << "}";
                
                return lhs;
            }
        };
    }
    
    typedef memory::Info MemoryInfo;
}
