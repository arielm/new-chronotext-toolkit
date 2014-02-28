/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/path/SplinePath.h"
#include "chronotext/path/ASPC.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    SplinePath::SplinePath(int capacity)
    {
        if (capacity > 0)
        {
            points.reserve(capacity);
        }
    }

    void SplinePath::add(const Vec2f &point)
    {
        points.emplace_back(point);
    }

    void SplinePath::add(float x, float y)
    {
        points.emplace_back(x, y);
    }
    
    void SplinePath::clear()
    {
        points.clear();
    }
    
    void SplinePath::flush(function<ci::Vec2f (float, ci::Vec2f*)> gamma, FollowablePath &path, float tol)
    {
        ASPC aspc(gamma, path, tol);
        
//        switch (type)
//        {
//            case TYPE_BSPLINE:
//                aspc.gamma = GammaBSpline;
//                break;
//                
//            case TYPE_CATMULL_ROM:
//                aspc.gamma = GammaCatmullRom;
//                break;
//                
//            default:
//                return;
//        }
        
        for (int i = 0, end = points.size() - 3; i < end; i++)
        {
            aspc.segment(&points[i]);
        }

        clear();
    }
}
