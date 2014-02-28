/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
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
    class FollowablePath
    {
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
        
        typedef enum
        {
            MODE_BOUNDED,
            MODE_LOOP,
            MODE_TANGENT,
            MODE_MODULO,
        }
        Mode;
        
        int mode;
        int size;
        
        std::vector<ci::Vec2f> points;
        std::vector<float> len;
        
        FollowablePath(Mode mode = MODE_TANGENT, int capacity = 0);
        FollowablePath(const std::vector<ci::Vec2f> &points, Mode mode = MODE_TANGENT);
        FollowablePath(ci::DataSourceRef source, Mode mode = MODE_TANGENT);

        void read(ci::DataSourceRef source);
        void write(ci::DataTargetRef target);

        void add(const std::vector<ci::Vec2f> &points);
        void add(const ci::Vec2f &point);
        inline void add(float x, float y) { add(ci::Vec2f(x, y)); }

        void clear();
        float getLength() const;

        Value pos2Value(float pos) const;
        ci::Vec2f pos2Point(float pos) const;
        float pos2Angle(float pos) const;
        float pos2SampledAngle(float pos, float sampleSize) const;
        ci::Vec2f pos2Gradient(float pos, float sampleSize) const;
        
        bool findClosestPoint(const ci::Vec2f &point, float threshold, ClosePoint &res) const;
        ClosePoint closestPointFromSegment(const ci::Vec2f &point, int segmentIndex) const;
        
        ci::Rectf getBounds() const;
        
    protected:
        void extendCapacity(int amount);
    };
}

namespace chr = chronotext;
