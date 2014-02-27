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
    SplinePath::SplinePath(const function<Vec2f (float, Vec2f*)> &gamma, float tol, int capacity)
    :
    gamma(gamma),
    tol(tol)
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
    
    void SplinePath::flush(FollowablePath &path)
    {
        ASPC aspc(path, gamma, tol);
        
        for (int i = 0, end = points.size() - 3; i < end; i++)
        {
            aspc.segment(&points[i]);
        }
        
        if (path.mode == FollowablePath::MODE_LOOP)
        {
            path.add(path.points.front());
        }
    }
}
