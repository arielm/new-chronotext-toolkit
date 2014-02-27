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
    ASPC::ASPC(float tol, const function<Vec2f (float, Vec2f*)> &gamma, FollowablePath &path)
    :
    tol(tol),
    gamma(gamma),
    path(path)
    {}

    void ASPC::segment(Vec2f *point)
    {
        this->point = point;
        
        float pt = 0;
        auto p = gamma(pt, point);
        
        float qt = 1;
        auto q = gamma(qt, point);
        
        sample(pt, p.x, p.y, qt, q.x, q.y);
    }
    
    void ASPC::sample(float t0, float x0, float y0, float t1, float x1, float y1)
    {
        float t = 0.45f + 0.1f * rand() / RAND_MAX;
        float rt = t0 + t * (t1 - t0);
        auto r = gamma(rt, point);
        
        float cross = (x0 - r.x) * (y1 - r.y) - (x1 - r.x) * (y0 - r.y);
        
        if (cross * cross < tol)
        {
            path.add(x0, y0);
        }
        else
        {
            sample(t0, x0, y0, rt, r.x, r.y);
            sample(rt, r.x, r.y, t1, x1, y1);
        }
    }
}
