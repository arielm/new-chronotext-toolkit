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
        int size = points.size();
        
        if (size > 2)
        {
            ASPC aspc(gamma, path, tol);
            
            aspc.segment(points[0], points[0], points[0], points[1]);
            aspc.segment(points[0], points[0], points[1], points[2]);
            
            for (int i = 0; i < size - 3; i++)
            {
                aspc.segment(points[i], points[i + 1], points[i + 2], points[i + 3]);
            }
            
            aspc.segment(points[size - 3], points[size - 2], points[size - 1], points[size - 1]);
            aspc.segment(points[size - 2], points[size - 1], points[size - 1], points[size - 1]);
            aspc.segment(points[size - 1], points[size - 1], points[size - 1], points[size - 1]);
        }
    }
}
