/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/path/SplinePath.h"
#include "chronotext/path/ASPC.h"

using namespace std;

namespace chronotext
{
    SplinePath::SplinePath(const function<float (float, float*)> &gamma, float tol, int capacity)
    :
    gamma(gamma),
    tol(tol),
    size(0)
    {
        x.reserve(capacity);
        y.reserve(capacity);
    }

    void SplinePath::add(float xx, float yy)
    {
        x.push_back(xx);
        y.push_back(yy);
        size++;
    }
    
    void SplinePath::clear()
    {
        size = 0;
        x.clear();
        y.clear();
    }
    
    void SplinePath::compute(FollowablePath *path)
    {
        ASPC aspc(tol, gamma, path);
        
        for (int i = 0; i < size - 3; i++)
        {
            aspc.segment(&x[i], &y[i]);
        }
        
        if (path->mode == FollowablePath::MODE_LOOP)
        {
            path->add(path->points.front());
        }
    }
}
