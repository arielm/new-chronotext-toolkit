/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Rect.h"
#include "cinder/DataSource.h"
#include "cinder/DataTarget.h"

namespace chronotext
{
    typedef std::shared_ptr<class FollowablePath> FollowablePathRef;
    
    class FollowablePath
    {
        void read(ci::IStreamRef in);
        void write(ci::OStreamRef out);
        
    public:
        struct Value
        {
            ci::Vec2f point;
            float angle;
            float position;
        };
        
        struct ClosePoint
        {
            ci::Vec2f point; // CLOSEST-POINT ON PATH
            float position; // POSITION OF CLOSEST-POINT ON PATH
            float distance; // DISTANCE TO CLOSEST-POINT ON PATH
        };
        
        enum
        {
            MODE_BOUNDED,
            MODE_LOOP,
            MODE_TANGENT,
            MODE_MODULO,
        };
        
        int mode;
        int size;
        
        std::vector<ci::Vec2f> points;
        std::vector<float> len;
        
        FollowablePath(int mode = MODE_TANGENT, int capacity = 256);
        FollowablePath(ci::DataSourceRef source, int mode = MODE_TANGENT);
        FollowablePath(const ci::Buffer &buffer, int mode = MODE_TANGENT);
        
        void write(ci::DataTargetRef target);
        ci::Buffer write();
        
        void clear();
        float getLength();
        
        void add(const ci::Vec2f &point);
        inline void add(float x, float y) { add(ci::Vec2f(x, y)); }
        
        Value pos2Value(float pos) const;
        ci::Vec2f pos2Point(float pos) const;
        float pos2Angle(float pos) const;
        float pos2SampledAngle(float pos, float sampleSize) const;
        ci::Vec2f pos2Gradient(float pos, float sampleSize) const;
        
        bool findClosestPoint(const ci::Vec2f &point, float min, ClosePoint &res) const;
        ClosePoint closestPointFromSegment(const ci::Vec2f &point, int segmentIndex) const;
        
        ci::Rectf getBounds() const;
    };
}

namespace chr = chronotext;
