/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/utils/Utils.h"

using namespace std;

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
            
            friend ostream& operator<<(ostream &lhs, const Info &rhs)
            {
                lhs << "{";
                
                if (rhs.free > 0)
                {
                    lhs << "free: " << rhs.write(rhs.free);
                }
                
                if (rhs.used > 0)
                {
                    if (rhs.free > 0)
                    {
                        lhs << ", ";
                    }
                    
                    lhs << "used: " << rhs.write(rhs.used);
                }
                
                if ((rhs.free > 0) && (rhs.used > 0))
                {
                    double ratio = rhs.used / double(rhs.free + rhs.used);
                    
                    lhs << ", percent: " << percent(ratio);
                }
                
                lhs << "}";
                
                return lhs;
            }
            
            static string write(int64_t bytes, int precision = 1, double unit = 1024 * 1024, const std::string &suffix = "M")
            {
                if (bytes <= 0)
                {
                    return "";
                }
                
                stringstream s;
                s << fixed << setprecision(precision) << bytes / unit << suffix;
                return s.str();
            }
        };
    }
    
    typedef memory::Info MemoryInfo;
}
