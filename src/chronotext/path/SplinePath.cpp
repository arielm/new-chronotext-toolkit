/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/path/SplinePath.h"

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
        for (int i = 0, end = points.size() - 3; i < end; i++)
        {
            segment(&path, &points[i]);
        }
        
        if (path.mode == FollowablePath::MODE_LOOP)
        {
            path.add(path.points.front());
        }
    }
    
    void SplinePath::segment(FollowablePath *path, Vec2f *point)
    {
        float pt = 0;
        auto p = gamma(pt, point);
        
        float qt = 1;
        auto q = gamma(qt, point);
        
        sample(path, point, pt, p, qt, q);
    }
    
    void SplinePath::sample(FollowablePath *path, Vec2f *point, float t0, const Vec2f &p0, float t1, const Vec2f &p1)
    {
        float t = 0.45f + 0.1f * rand() / RAND_MAX;
        float rt = t0 + t * (t1 - t0);
        auto r = gamma(rt, point);
        
        float cross = (p0 - r).cross(p1 - r);
        
        if (cross * cross < tol)
        {
            path->add(p0);
        }
        else
        {
            sample(path, point, t0, p0, rt, r);
            sample(path, point, rt, r, t1, p1);
        }
    }
}
