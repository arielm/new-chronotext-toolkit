/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/path/ASPC.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    ASPC::ASPC(FollowablePath &path, const function<Vec2f (float, Vec2f*)> &gamma, float tol)
    :
    path(path),
    gamma(gamma),
    tol(tol)
    {}
    
    void ASPC::segment(Vec2f *point)
    {
        this->point = point;
        
        float pt = 0;
        auto p = gamma(pt, point);
        
        float qt = 1;
        auto q = gamma(qt, point);
        
        sample(pt, p, qt, q);
    }
    
    void ASPC::sample(float t0, const Vec2f &p0, float t1, const Vec2f &p1)
    {
        float t = 0.45f + 0.1f * rand() / RAND_MAX;
        float rt = t0 + t * (t1 - t0);
        auto r = gamma(rt, point);
        
        float cross = (p0 - r).cross(p1 - r);
        
        if (cross * cross < tol)
        {
            path.add(p0);
        }
        else
        {
            sample(t0, p0, rt, r);
            sample(rt, r, t1, p1);
        }
    }
}
