#include "chronotext/path/SplinePath.h"
#include "chronotext/path/ASPC.h"

using namespace std;

SplinePath::SplinePath(const function<float (float, float*)> &gamma, float tol, int capacity)
:
gamma(gamma),
tol(tol),
size(0)
{
    x.reserve(capacity);
    y.reserve(capacity);
}

void SplinePath::clear()
{
    size = 0;
    x.clear();
    y.clear();
}

void SplinePath::add(float xx, float yy)
{
    x.push_back(xx);
    y.push_back(yy);
    
    size++;
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
        path->add(path->x[0], path->y[0]);
    }
}
