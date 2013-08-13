#include "chronotext/path/FollowablePath.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/utils/Utils.h"

using namespace ci;
using namespace std;

FollowablePath::FollowablePath(int mode, int capacity)
:
mode(mode),
size(0)
{
    x.reserve(capacity);
    y.reserve(capacity);
    len.reserve(capacity);
}

FollowablePath::FollowablePath(DataSourceRef source, int mode)
:
mode(mode)
{
    read(source->createStream());
}

FollowablePath::FollowablePath(const Buffer &buffer, int mode)
:
mode(mode)
{
    IStreamRef in = IStreamMem::create(buffer.getData(), buffer.getDataSize());
    read(in);
}

/*
 * ASSERTION: THIS IS CALLED AT CONSTRUCTION TIME
 */
void FollowablePath::read(IStreamRef in)
{
    int capacity;
    in->readLittle(&capacity);
    
    size = 0;
    x.reserve(capacity);
    y.reserve(capacity);
    len.reserve(capacity);
    
    float xx;
    float yy;
    
    for (int i = 0; i < capacity; i++)
    {
        in->readLittle(&xx);
        in->readLittle(&yy);
        add(xx, yy);
    }
}

void FollowablePath::write(OStreamRef out)
{
    out->writeLittle(size);
    
    for (int i = 0; i < size; i++)
    {
        out->writeLittle(x[i]);
        out->writeLittle(y[i]);
    }
}

void FollowablePath::write(DataTargetRef target)
{
    write(target->getStream());
}

Buffer FollowablePath::write()
{
    int bufferSize = sizeof(int) + 2 * size * sizeof(float);
    OStreamMemRef out = OStreamMem::create(bufferSize);
    
    write(out);
    
    Buffer buffer(bufferSize);
    buffer.copyFrom(out->getBuffer(), bufferSize);
    return buffer;
}

void FollowablePath::clear()
{
    size = 0;
    x.clear();
    y.clear();
    len.clear();
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

void FollowablePath::add(float xx, float yy)
{
    x.push_back(xx);
    y.push_back(yy);
    
    if (size > 0)
    {
        float dx = xx - x[size - 1];
        float dy = yy - y[size - 1];
        len.push_back(len[size - 1] + math<float>::sqrt(dx * dx + dy * dy));
    }
    else
    {
        len.push_back(0);
    }
    
    size++;
}

FollowablePath::Value FollowablePath::pos2Value(float pos) const
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
    
    float ratio = (pos - len[index]) / (len[index + 1] - len[index]);
    
    FollowablePath::Value value;
    value.x = x0 + (x1 - x0) * ratio;
    value.y = y0 + (y1 - y0) * ratio;
    value.angle = math<float>::atan2(y1 - y0, x1 - x0);
    value.position = pos;
    
    return value;
}

Vec2f FollowablePath::pos2Point(float pos) const
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
                return Vec2f(x[0], y[0]);
            }
        }
        else if (pos >= length)
        {
            if (mode == MODE_BOUNDED)
            {
                return Vec2f(x[size - 1], y[size - 1]);
            }
        }
    }
    
    int index = search(len, pos, 1, size);
    
    float x0 = x[index];
    float y0 = y[index];
    
    float x1 = x[index + 1];
    float y1 = y[index + 1];
    
    float ratio = (pos - len[index]) / (len[index + 1] - len[index]);
    return Vec2f(x0 + (x1 - x0) * ratio, y0 + (y1 - y0) * ratio);
}

float FollowablePath::pos2Angle(float pos) const
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
    
    return math<float>::atan2(y1 - y0, x1 - x0);
}

float FollowablePath::pos2SampledAngle(float pos, float sampleSize) const
{
    Vec2f gradient = pos2Gradient(pos, sampleSize);
    
    /*
     * WE USE AN EPSILON VALUE TO AVOID
     * DEGENERATED RESULTS IN SOME EXTREME CASES
     * (E.G. CLOSE TO 180 DEGREE DIFF. BETWEEN TWO SEGMENTS)
     */
    if (gradient.lengthSquared() > 1.0)
    {
        return math<float>::atan2(gradient.y, gradient.x);
    }
    else
    {
        return pos2Angle(pos);
    }
}

Vec2f FollowablePath::pos2Gradient(float pos, float sampleSize) const
{
    Vec2f pm = pos2Point(pos - sampleSize * 0.5f);
    Vec2f pp = pos2Point(pos + sampleSize * 0.5f);

    return (pp - pm) * 0.5f;
}

/*
 * RETURNS false IF CLOSEST POINT IS FARTHER THAN min DISTANCE
 * 
 * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
 * http://paulbourke.net/geometry/pointlineplane/
 */
bool FollowablePath::findClosestPoint(float xx, float yy, float min, FollowablePath::ClosePoint &res) const
{
    min *= min; // BECAUSE COMPARING "MAGNIFIED DISTANCES" IS FASTER
    
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
        res.x = _x;
        res.y = _y;
        res.position = _len;
        res.distance = math<float>::sqrt(min);
        
        return true;
    }
    
    return false;
}

/*
 * segmentIndex MUST BE < size
 * 
 * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
 * http://paulbourke.net/geometry/pointlineplane/
 */
FollowablePath::ClosePoint FollowablePath::closestPointFromSegment(float xx, float yy, int segmentIndex) const
{
    FollowablePath::ClosePoint res;
    
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
        
        res.x = xp;
        res.y = yp;
        res.position = len[i0] + d * u;
        res.distance = math<float>::sqrt(mag);
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
            res.x = x0;
            res.y = y0;
            res.position = len[i0];
            res.distance = math<float>::sqrt(mag0);
        }
        else
        {
            res.x = x1;
            res.y = y1;
            res.position = len[i1];
            res.distance = math<float>::sqrt(mag1);
        }
    }
    
    return res;
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
