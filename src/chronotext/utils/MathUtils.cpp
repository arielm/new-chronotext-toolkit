#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

static float linearYatX(const Vec2f p[2], float x)
{
	if (p[0].x == p[1].x) return p[0].y;
	return p[0].y + (p[1].y - p[0].y) * (x - p[0].x) / (p[1].x - p[0].x);
}

static size_t linearCrossings(const Vec2f p[2], const Vec2f &pt)
{
	if ((p[0].x < pt.x && pt.x <= p[1].x) || (p[1].x < pt.x && pt.x <= p[0].x ))
    {
		if (pt.y > linearYatX(p, pt.x)) return 1;
	}
    
	return 0;
}

bool isPointInside(const Vec2f &point, const vector<Vec2f> &points)
{
    if (points.size() <= 2)
    {
		return false;
    }
    else
    {
        size_t crossings = 0;
        
        for (size_t s = 0; s < points.size() - 1; ++s)
        {
            crossings += linearCrossings( &(points[s]), point);
        }
        
        Vec2f temp[2];
        temp[0] = points[points.size() - 1];
        temp[1] = points[0];
        crossings += linearCrossings(&(temp[0]), point);
        
        return (crossings & 1) == 1;
    }
}
