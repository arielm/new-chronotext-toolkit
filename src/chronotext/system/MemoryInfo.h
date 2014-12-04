/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <iomanip> 

using namespace std;

namespace chr
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
            
            friend ostream& operator<<(ostream &lhs, const Info &rhs)
            {
                lhs << "{";
                
                if (rhs.free > 0)
                {
                    lhs << "free: " << rhs.write(rhs.free);
                }
                
                if (rhs.used > 0)
                {
                    lhs << ", used: " << rhs.write(rhs.used);
                }
                
                if (rhs.ratio > 0)
                {
                    lhs << ", ratio: " << rhs.write(rhs.ratio);
                }
                
                lhs << "}";
                
                return lhs;
            }
            
            string write(int64_t bytes, int precision = 2, double unit = 1024 * 1024, const std::string &suffix = "") const
            {
                if (bytes < 0)
                {
                    return "";
                }
                
                stringstream s;
                
                s << fixed << setprecision(precision) << bytes / unit;
                
                if (!suffix.empty())
                {
                    s << " " << suffix;
                }
                
                return s.str();
            }
        };
    }
}
