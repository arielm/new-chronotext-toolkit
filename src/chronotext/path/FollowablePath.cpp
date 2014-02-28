/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/path/FollowablePath.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/utils/Utils.h"

using namespace ci;
using namespace std;

namespace chronotext
{
    FollowablePath::FollowablePath(Mode mode, int capacity)
    :
    mode(mode),
    size(0)
    {
        if (capacity > 0)
        {
            points.reserve(capacity);
            len.reserve(capacity);
        }
    }
    
    FollowablePath::FollowablePath(const vector<Vec2f> &points, Mode mode)
    :
    mode(mode),
    size(0)
    {
        add(points);
    }
    
    FollowablePath::FollowablePath(DataSourceRef source, Mode mode)
    :
    mode(mode),
    size(0)
    {
        read(source);
    }
    
    void FollowablePath::read(DataSourceRef source)
    {
        auto stream = source->createStream();
        
        int newPointsSize;
        stream->readLittle(&newPointsSize);
        
        extendCapacity(newPointsSize);
        
        Vec2f point;
        
        for (int i = 0; i < newPointsSize; i++)
        {
            stream->readLittle(&point.x);
            stream->readLittle(&point.y);
            add(point);
        }
    }
    
    void FollowablePath::write(DataTargetRef target)
    {
        auto stream = target->getStream();
        
        stream->writeLittle(size);
        
        for (auto &point : points)
        {
            stream->writeLittle(point.x);
            stream->writeLittle(point.y);
        }
    }
    
    void FollowablePath::add(const vector<Vec2f> &newPoints)
    {
        extendCapacity(newPoints.size());
        
        for (auto &point : newPoints)
        {
            add(point);
        }
    }
    
    void FollowablePath::add(const ci::Vec2f &point)
    {
        if (size > 0)
        {
            Vec2f delta = point - points[size - 1];
            
            if (delta != Vec2f::zero())
            {
                len.push_back(len[size - 1] + delta.length());
            }
            else
            {
                return;
            }
        }
        else
        {
            len.push_back(0);
        }

        points.push_back(point);
        size++;
    }
    
    void FollowablePath::clear()
    {
        size = 0;
        points.clear();
        len.clear();
    }
    
    float FollowablePath::getLength() const
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
    
    FollowablePath::Value FollowablePath::pos2Value(float pos) const
    {
        float length = len[size - 1];
        
        if (mode == MODE_LOOP || mode == MODE_MODULO)
        {
            pos = boundf(pos, length);
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
        auto p0 = points[index];
        auto p1 = points[index + 1];
        
        float ratio = (pos - len[index]) / (len[index + 1] - len[index]);
        
        FollowablePath::Value value;
        value.point = p0 + (p1 - p0) * ratio;
        value.angle = math<float>::atan2(p1.y - p0.y, p1.x - p0.x);
        value.position = pos;
        
        return value;
    }
    
    Vec2f FollowablePath::pos2Point(float pos) const
    {
        float length = len[size - 1];
        
        if (mode == MODE_LOOP || mode == MODE_MODULO)
        {
            pos = boundf(pos, length);
        }
        else
        {
            if (pos <= 0)
            {
                if (mode == MODE_BOUNDED)
                {
                    return points[0];
                }
            }
            else if (pos >= length)
            {
                if (mode == MODE_BOUNDED)
                {
                    return points[size - 1];
                }
            }
        }
        
        int index = search(len, pos, 1, size);
        auto p0 = points[index];
        auto p1 = points[index + 1];
        
        float ratio = (pos - len[index]) / (len[index + 1] - len[index]);
        return p0 + (p1 - p0) * ratio;
    }
    
    float FollowablePath::pos2Angle(float pos) const
    {
        float length = len[size - 1];
        
        if (mode == MODE_LOOP || mode == MODE_MODULO)
        {
            pos = boundf(pos, length);
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
        auto p0 = points[index];
        auto p1 = points[index + 1];
        
        return math<float>::atan2(p1.y - p0.y, p1.x - p0.x);
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
     * RETURNS false IF CLOSEST POINT IS FARTHER THAN threshold DISTANCE
     *
     * REFERENCE: "Minimum Distance between a Point and a Line" BY Paul Bourke
     * http://paulbourke.net/geometry/pointlineplane/
     */
    bool FollowablePath::findClosestPoint(const Vec2f &point, float threshold, FollowablePath::ClosePoint &res) const
    {
        float min = threshold * threshold; // BECAUSE IT IS MORE EFFICIENT TO WORK WITH MAGNIFIED DISTANCES
        
        int index = -1;
        Vec2f _point;
        float _len;
        
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
            
            auto p0 = points[i0];
            auto p1 = points[i1];
            
            Vec2f delta = p1 - p0;
            float l = len[i1] - len[i0];
            float u = delta.dot(point - p0) / (l * l);
            
            if (u >= 0 && u <= 1)
            {
                Vec2f p = p0 + u * delta;
                float mag = (p - point).lengthSquared();
                
                if (mag < min)
                {
                    min = mag;
                    index = i0;
                    
                    _point = p;
                    _len = len[index] + u * l;
                }
            }
            else
            {
                float mag0 = (p0 - point).lengthSquared();
                float mag1 = (p1 - point).lengthSquared();
                
                if ((mag0 < min) && (mag0 < mag1))
                {
                    min = mag0;
                    index = i0;
                    
                    _point = points[i0];
                    _len = len[index];
                }
                else if ((mag1 < min) && (mag1 < mag0))
                {
                    min = mag1;
                    index = i1;
                    
                    _point = points[i1];
                    _len = len[index];
                }
            }
        }
        
        if (index != -1)
        {
            res.point = _point;
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
    FollowablePath::ClosePoint FollowablePath::closestPointFromSegment(const Vec2f &point, int segmentIndex) const
    {
        FollowablePath::ClosePoint res;
        
        int i0 = segmentIndex;
        int i1 = segmentIndex + 1;
        
        auto p0 = points[i0];
        auto p1 = points[i1];
        
        Vec2f delta = p1 - p0;
        float l = len[i1] - len[i0];
        float u = delta.dot(point - p0) / (l * l);
        
        if (u >= 0 && u <= 1)
        {
            Vec2f p = p0 + u * delta;
            float mag = (p - point).lengthSquared();
            
            res.point = p;
            res.position = len[i0] + u * l;
            res.distance = math<float>::sqrt(mag);
        }
        else
        {
            float mag0 = (p0 - point).lengthSquared();
            float mag1 = (p1 - point).lengthSquared();
            
            if (mag0 < mag1)
            {
                res.point = p0;
                res.position = len[i0];
                res.distance = math<float>::sqrt(mag0);
            }
            else
            {
                res.point = p1;
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
        
        for (auto &point : points)
        {
            if (point.x < minX) minX = point.x;
            if (point.y < minY) minY = point.y;
            
            if (point.x > maxX) maxX = point.x;
            if (point.y > maxY) maxY = point.y;
        }
        
        return Rectf(minX, minY, maxX, maxY);
    }
    
    void FollowablePath::extendCapacity(int amount)
    {
        int newCapacity = size + amount;
        points.reserve(newCapacity);
        len.reserve(newCapacity);
    }
}
