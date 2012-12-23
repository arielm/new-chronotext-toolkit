#include "chronotext/path/FollowablePath.h"

using namespace ci;
using namespace std;

#define CAPACITY_INCREMENT 0 /* 0 MEANS THAT CAPACITY IS MULTIPLIED BY 2 WHEN NECESSARY */

static int search(float *array, float value, int min, int max)
{
    int mid = (min + max) >> 1;
    
    while (min < mid)
    {
        if (array[mid - 1] < value)
        {
            min = mid;
        }
        else if (array[mid - 1] > value)
        {
            max = mid;
        }
        else
        {
            min = max = mid;
        }
        mid = (min + max) >> 1;
    }
    
    return mid - 1;
}

static inline float bound(float value, float range)
{
    float bound = fmod(value, range);
    return (bound < 0) ? (bound + range) : bound;
}

FollowablePath::FollowablePath(int mode, int capacity)
:
mode(mode),
capacity(capacity),
size(0)
{
    x = new float[capacity];
    y = new float[capacity];
    len = new float[capacity];
}

FollowablePath::~FollowablePath()
{
    delete[] x;
    delete[] y;
    delete[] len;
}

void FollowablePath::ensureCapacity(int minCapacity)
{
    if (minCapacity > capacity)
    {
        capacity = (CAPACITY_INCREMENT > 0) ? (capacity + CAPACITY_INCREMENT) : (capacity * 2);
        
        x = (float*) realloc(x, capacity * sizeof(float));
        y = (float*) realloc(y, capacity * sizeof(float));
        len = (float*) realloc(len, capacity * sizeof(float));
    }
}

void FollowablePath::add(float xx, float yy)
{
    ensureCapacity(size + 1);
    
    x[size] = xx;
    y[size] = yy;
    
    if (size > 0)
    {
        float dx = xx - x[size - 1];
        float dy = yy - y[size - 1];
        len[size] = len[size - 1] + sqrtf(dx * dx + dy * dy);
    }
    else
    {
        len[0] = 0;
    }
    
    size++;
}

float FollowablePath::getLength()
{
    if (size > 0)
    {
        return len[size - 1];
    }
    else
    {
        return 0;
    }
}

void FollowablePath::clear()
{
    size = 0;
}

void FollowablePath::pos2Point(float pos, float *res)
{
    float length = len[size - 1];
    
    if (mode == MODE_LOOP || mode == MODE_MODULO)
    {
        pos = bound(pos, length);
    }
    else
    {
        if (pos <= 0)
        {
            if (mode == MODE_BOUNDED)
            {
                *res++ = x[0];
                *res++ = y[0];
                *res   = 0;
                return;
            }
        }
        else if (pos >= length)
        {
            if (mode == MODE_BOUNDED)
            {
                *res++ = x[size - 1];
                *res++ = y[size - 1];
                *res   = length;
                return;
            }
        }
    }
    
    int index = search(len, pos, 1, size);
    
    float x0 = x[index];
    float y0 = y[index];
    
    float x1 = x[index + 1];
    float y1 = y[index + 1];
    
    float ratio = (pos - len[index]) / (len[index + 1] - len[index]);
    *res++ = x0 + (x1 - x0) * ratio;
    *res++ = y0 + (y1 - y0) * ratio;
    *res   = pos;
}

float FollowablePath::pos2Angle(float pos)
{
    float length = len[size - 1];
    
    if (mode == MODE_LOOP || mode == MODE_MODULO)
    {
        pos = bound(pos, length);
    }
    else
    {
        if (pos <= 0)
        {
            if (mode == MODE_BOUNDED)
            {
                pos = 0;
            }
        }
        else if (pos >= length)
        {
            if (mode == MODE_BOUNDED)
            {
                pos = length;
            }
        }
    }
    
    int index = search(len, pos, 1, size);
    
    float x0 = x[index];
    float y0 = y[index];
    
    float x1 = x[index + 1];
    float y1 = y[index + 1];
    
    return atan2(y1 - y0, x1 - x0);
}

float FollowablePath::pos2SampledAngle(float pos, float sampleSize)
{
    float res[2];
    pos2Gradient(pos, sampleSize, res);
    float dx = res[0];
    float dy = res[1];
    
    /*
     * WE USE AN EPSILON VALUE TO AVOID
     * DEGENERATED RESULTS IN SOME EXTREME CASES
     * (E.G. CLOSE TO 180 DEGREE DIFF. BETWEEN TWO SEGMENTS)
     */
    if ((dx * dx + dy * dy) > 1.0)
    {
        return atan2(dy, dx);
    }
    else
    {
        return pos2Angle(pos);
    }
}

void FollowablePath::pos2Gradient(float pos, float sampleSize, float *res)
{
    float halfSampleSize = sampleSize / 2;
    
    float pm[3];
    pos2Point(pos - halfSampleSize, pm);
    
    float pp[3];
    pos2Point(pos + halfSampleSize, pp);
    
    float dx = pp[0] - pm[0];
    float dy = pp[1] - pm[1];
    
    *res++ = dx / 2;
    *res   = dy / 2;
}

/*
 * RETURNS false IF CLOSEST POINT IS FARTHER THAN min DISTANCE
 *
 * res[0]: CLOSEST-POINT X
 * res[1]: CLOSEST-POINT Y
 * res[2]: POSITION OF CLOSEST-POINT
 * res[3]: DISTANCE TO CLOSEST-POINT
 * 
 * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
 * http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline
 */
bool FollowablePath::findClosestPoint(float xx, float yy, float min, float *res)
{
    min *= min; // BECAUSE WE COMPARE "MAGNIFIED DISTANCES" (FASTER...)
    
    int index = -1;
    float _x, _y, _len;
    
    for (int i = 0; i < size; i++)
    {
        int i0, i1;
        if (i == size - 1)
        {
            i0 = i - 1;
            i1 = i;
        }
        else
        {
            i0 = i;
            i1 = i + 1;
        }
        
        float d = len[i1] - len[i0];
        float x0 = x[i0];
        float y0 = y[i0];
        float x1 = x[i1];
        float y1 = y[i1];
        
        float u = ((xx - x0) * (x1 - x0) + (yy - y0) * (y1 - y0)) / (d * d);
        if (u >= 0 && u <= 1)
        {
            float xp = x0 + u * (x1 - x0);
            float yp = y0 + u * (y1 - y0);
            
            float dx = xp - xx;
            float dy = yp - yy;
            float mag = dx * dx + dy * dy;
            
            if (mag < min)
            {
                min = mag;
                index = i0;
                
                _x = xp;
                _y = yp;
                _len = len[index] + d * u;
            }
        }
        else
        {
            float dx0 = x0 - xx;
            float dy0 = y0 - yy;
            float mag1 = dx0 * dx0 + dy0 * dy0;
            
            float dx1 = x1 - xx;
            float dy1 = y1 - yy;
            float mag2 = dx1 * dx1 + dy1 * dy1;
            
            if ((mag1 < min) && (mag1 < mag2))
            {
                min = mag1;
                index = i0;
                
                _x = x0;
                _y = y0;
                _len = len[index];
            }
            else if ((mag2 < min) && (mag2 < mag1))
            {
                min = mag2;
                index = i1;
                
                _x = x1;
                _y = y1;
                _len = len[index];
            }
        }
    }
    
    if (index != -1)
    {
        res[0] = _x;
        res[1] = _y;
        res[2] = _len;
        res[3] = sqrt(min);
        
        return true;
    }
    
    return false;
}

/*
 * segmentIndex MUST BE < size
 *
 * res[0]: CLOSEST-POINT X
 * res[1]: CLOSEST-POINT Y
 * res[2]: POSITION OF CLOSEST-POINT
 * res[3]: DISTANCE TO CLOSEST-POINT
 * 
 * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
 * http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline
 */
void FollowablePath::closestPointFromSegment(float xx, float yy, int segmentIndex, float *res)
{
    int i0 = segmentIndex;
    int i1 = segmentIndex + 1;
    
    float d = len[i1] - len[i0];
    float x0 = x[i0];
    float y0 = y[i0];
    float x1 = x[i1];
    float y1 = y[i1];
    
    float u = ((xx - x0) * (x1 - x0) + (yy - y0) * (y1 - y0)) / (d * d);
    if (u >= 0 && u <= 1)
    {
        float xp = x0 + u * (x1 - x0);
        float yp = y0 + u * (y1 - y0);
        
        float dx = xp - xx;
        float dy = yp - yy;
        float mag = dx * dx + dy * dy;
        
        *res++ = xp;
        *res++ = yp;
        *res++ = len[i0] + d * u;
        *res   = sqrt(mag);
    }
    else
    {
        float dx0 = x0 - xx;
        float dy0 = y0 - yy;
        float mag0 = dx0 * dx0 + dy0 * dy0;
        
        float dx1 = x1 - xx;
        float dy1 = y1 - yy;
        float mag1 = dx1 * dx1 + dy1 * dy1;
        
        if (mag0 < mag1)
        {
            *res++ = x0;
            *res++ = y0;
            *res++ = len[i0];
            *res   = sqrt(mag0);
        }
        else
        {
            *res++ = x1;
            *res++ = y1;
            *res++ = len[i1];
            *res   = sqrt(mag1);
        }
    }
}

Rectf FollowablePath::getBounds() const
{
    float minX = numeric_limits<float>::max();
    float minY = numeric_limits<float>::max();
    float maxX = numeric_limits<float>::min();
    float maxY = numeric_limits<float>::min();
    
    for (int i = 0; i < size; i++)
    {
        float xx = x[i];
        float yy = y[i];
        
        if (xx < minX) minX = xx;
        if (yy < minY) minY = yy;
        
        if (xx > maxX) maxX = xx;
        if (yy > maxY) maxY = yy;
    }
    
    return Rectf(minX, minY, maxX, maxY);
}
