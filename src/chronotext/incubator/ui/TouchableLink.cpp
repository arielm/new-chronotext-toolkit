#include "chronotext/incubator/ui/TouchableLink.h"

using namespace ci;

namespace chronotext
{
    bool TouchableLink::hitTest(const Vec2f &point, float *distance)
    {
        Rectf extra = bounds.inflated(Vec2f(hitExtra, hitExtra));
        
        if (extra.contains(point))
        {
            *distance = bounds.distance(point);
            return true;
        }
        
        return false;
    }
}
