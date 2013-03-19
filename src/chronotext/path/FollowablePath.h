#pragma once

#include "cinder/Rect.h"
#include "cinder/DataSource.h"
#include "cinder/DataTarget.h"

typedef std::shared_ptr<class FollowablePath> FollowablePathRef;

class FollowablePath
{
    void read(ci::IStreamRef in);
    void write(ci::OStreamRef out);
    
public:
    struct Value
    {
        float x;
        float y;
        float angle;
        float position;
    };
    
    struct ClosePoint
    {
        float x; // CLOSEST-POINT ON PATH
        float y; // CLOSEST-POINT ON PATH
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
    
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> len;
    
    FollowablePath(int mode = MODE_TANGENT, int capacity = 256);
    FollowablePath(ci::DataSourceRef source, int mode = MODE_TANGENT);
    FollowablePath(const ci::Buffer &buffer, int mode = MODE_TANGENT);
    
    void write(ci::DataTargetRef target);
    ci::Buffer write();
    
    void clear();
    float getLength();
    void add(float x, float y);
    
    Value pos2Value(float pos) const;
    ci::Vec2f pos2Point(float pos) const;
    float pos2Angle(float pos) const;
    float pos2SampledAngle(float pos, float sampleSize) const;
    ci::Vec2f pos2Gradient(float pos, float sampleSize) const;
    
    bool findClosestPoint(float x, float y, float min, ClosePoint &res) const;
    ClosePoint closestPointFromSegment(float x, float y, int segmentIndex) const;
    
    ci::Rectf getBounds() const;
};
