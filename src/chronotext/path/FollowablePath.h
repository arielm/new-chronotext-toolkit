/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Path2d.h"
#include "cinder/DataSource.h"
#include "cinder/DataTarget.h"

namespace chr
{
    class FollowablePath
    {
    public:
        struct Value
        {
            ci::Vec2f position;
            float angle;
            float offset;
            int index;
        };
        
        struct ClosePoint
        {
            ci::Vec2f position; // POSITION OF CLOSEST-POINT ON PATH
            float offset; // OFFSET OF CLOSEST-POINT ON PATH
            float distance; // DISTANCE TO CLOSEST-POINT ON PATH
        };
        
        enum Mode
        {
            MODE_BOUNDED,
            MODE_LOOP,
            MODE_TANGENT,
            MODE_MODULO,
        };
        
        FollowablePath(int capacity = 0);
        FollowablePath(const std::vector<ci::Vec2f> &points);
        FollowablePath(const ci::Path2d &path, float approximationScale = 1.0f);
        FollowablePath(ci::DataSourceRef source);

        void read(ci::DataSourceRef source);
        void write(ci::DataTargetRef target);

        void add(const std::vector<ci::Vec2f> &points);
        void add(const ci::Vec2f &point);
        inline void add(float x, float y) { add(ci::Vec2f(x, y)); }

        const std::vector<ci::Vec2f>& getPoints() const;
        const std::vector<float>& getLengths() const;
        
        void clear();
        int size() const;
        bool empty() const;
        
        float getLength() const;
        ci::Rectf getBounds() const;
        
        void close();
        bool isClosed() const;
        
        void setMode(Mode mode);
        Mode getMode() const;

        Value offset2Value(float offset) const;
        ci::Vec2f offset2Position(float offset) const;
        float offset2Angle(float offset) const;
        float offset2SampledAngle(float offset, float sampleSize) const;
        ci::Vec2f offset2Gradient(float offset, float sampleSize) const;
        
        bool findClosestPoint(const ci::Vec2f &input, float threshold, ClosePoint &output) const;
        ClosePoint closestPointFromSegment(const ci::Vec2f &input, int segmentIndex) const;
        
    protected:
        Mode mode;

        std::vector<ci::Vec2f> points;
        std::vector<float> lengths;

        void extendCapacity(int amount);
    };
}
