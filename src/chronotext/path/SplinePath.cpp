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
    
    void SplinePath::flush(function<Vec2f (float, Vec2f*)> gamma, FollowablePath &path, float tol)
    {
        ASPC aspc(gamma, path, tol);
        
        for (int i = 0; i < points.size() - 3; i++)
        {
            int i0 = i;
            int i1 = i + 1;
            int i2 = i + 2;
            int i3 = i + 3;
            
            aspc.segment(points[i0], points[i1], points[i2], points[i3]);
        }

    }
}
