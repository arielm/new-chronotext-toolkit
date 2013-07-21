#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

/*
 * THE FOLLOWING 4 FUNCTIONS ARE BASED ON CODE FROM Cinder:
 *
 * https://github.com/cinder/Cinder/blob/ae580c2cb0fc44d0a99b233dbefdf736f7093209/src/cinder/Path2d.cpp#L754-768
 * https://github.com/cinder/Cinder/blob/ae580c2cb0fc44d0a99b233dbefdf736f7093209/src/cinder/Path2d.cpp#L824-857
 * https://github.com/cinder/Cinder/blob/ae580c2cb0fc44d0a99b233dbefdf736f7093209/src/cinder/Shape2d.cpp#L135-144
 *
 * ASSERTION: THE IMPLIED WINDING IS "ODD" (AKA "EVEN-ODD FILL-RULE")
 *
 * TODO:
 * CHECK WHAT'S NECESSARY IN ORDER TO HANDLE "NON-ZERO" WINDING (WHICH SEEMS TO BE THE DEFAULT FILL-RULE IN HTML CANVAS)
 * REFERENCE: http://blogs.adobe.com/webplatform/2013/01/30/winding-rules-in-canvas/
 */

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

/*
 * A PATH IS MADE OF N POINTS
 */
bool isPointInside(const Vec2f &point, const vector<Vec2f> &path)
{
    const size_t size = path.size();
    
    if (size <= 2)
    {
		return false;
    }
    else
    {
        size_t crossings = 0;
        
        for (size_t s = 0; s < size - 1; ++s)
        {
            crossings += linearCrossings(&(path[s]), point);
        }
        
        Vec2f temp[2];
        temp[0] = path[size - 1];
        temp[1] = path[0];
        crossings += linearCrossings(&(temp[0]), point);
        
        return (crossings & 1) == 1;
    }
}

/*
 * A POLYGON IS MADE OF N PATHS
 */
bool isPointInside(const Vec2f &point, const vector<vector<Vec2f>> &polygon)
{
    int numPathsInside = 0;
    
	for (auto path : polygon)
    {
		if (isPointInside(point, path))
        {
			numPathsInside++;
        }
	}
    
	return (numPathsInside % 2) == 1;
}
