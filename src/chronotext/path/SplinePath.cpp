/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/path/SplinePath.h"
#include "chronotext/path/ASPC.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    SplinePath::SplinePath(int capacity)
    :
    closed(false)
    {
        if (capacity > 0)
        {
            points.reserve(capacity);
        }
    }
    
    SplinePath::SplinePath(const vector<Vec2f> &points)
    :
    closed(false)
    {
        add(points);
    }
    
    SplinePath::SplinePath(DataSourceRef source)
    :
    closed(false)
    {
        read(source);
    }
    
    void SplinePath::read(DataSourceRef source)
    {
        auto stream = source->createStream();
        
        int newPointsSize;
        stream->readLittle(&newPointsSize);
        
        points.reserve(size() + newPointsSize);
        
        // ---
        
        Vec2f point;
        
        for (int i = 0; i < newPointsSize; i++)
        {
            stream->readLittle(&point.x);
            stream->readLittle(&point.y);
            add(point);
        }
    }
    
    void SplinePath::write(DataTargetRef target)
    {
        auto stream = target->getStream();
        
        stream->writeLittle(size());
        
        for (auto &point : points)
        {
            stream->writeLittle(point.x);
            stream->writeLittle(point.y);
        }
    }
    
    void SplinePath::add(const vector<Vec2f> &newPoints)
    {
        points.reserve(size() + newPoints.size());
        
        for (auto &point : newPoints)
        {
            add(point);
        }
    }
    
    void SplinePath::add(const Vec2f &point)
    {
        if (!points.empty() && point == points.back())
        {
            return;
        }
        
        points.emplace_back(point);
    }
    
    const vector<Vec2f>& SplinePath::getPoints() const
    {
        return points;
    }
    
    void SplinePath::clear()
    {
        points.clear();
    }
    
    int SplinePath::size() const
    {
        return points.size();
    }
    
    bool SplinePath::empty() const
    {
        return points.empty();
    }
    
    void SplinePath::close()
    {
        if (size() > 2)
        {
            closed = true;
            
            if (points.front() == points.back())
            {
                points.pop_back();
            }
        }
    }
    
    bool SplinePath::isClosed() const
    {
        return closed;
    }
    
    void SplinePath::flush(Type type, FollowablePath &path, float tol)
    {
        function<Vec2f (float, Vec2f*)> gamma;
        
        switch (type)
        {
            case TYPE_BSPLINE:
                gamma = GammaBSpline;
                break;
                
            case TYPE_CATMULL_ROM:
                gamma = GammaCatmullRom;
                break;
                
            default:
                return;
        }
        
        int size = points.size();
        
        if (size > 2)
        {
            ASPC aspc(gamma, path, tol);
            
            if (closed)
            {
                aspc.segment(points[size - 1], points[0], points[1], points[2]);
            }
            else
            {
                if (type == TYPE_BSPLINE)
                {
                    aspc.segment(points[0], points[0], points[0], points[1]);
                }
                
                aspc.segment(points[0], points[0], points[1], points[2]);
            }

            for (int i = 0; i < size - 3; i++)
            {
                aspc.segment(points[i], points[i + 1], points[i + 2], points[i + 3]);
            }
            
            if (closed)
            {
                aspc.segment(points[size - 3], points[size - 2], points[size - 1], points[0]);
                aspc.segment(points[size - 2], points[size - 1], points[0], points[1]);
                
                path.close();
                path.setMode(FollowablePath::MODE_LOOP);
            }
            else
            {
                aspc.segment(points[size - 3], points[size - 2], points[size - 1], points[size - 1]);
                aspc.segment(points[size - 2], points[size - 1], points[size - 1], points[size - 1]);
                
                if (type == TYPE_BSPLINE)
                {
                    aspc.segment(points[size - 1], points[size - 1], points[size - 1], points[size - 1]);
                }
            }
        }
    }
}
