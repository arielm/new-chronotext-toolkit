#pragma once

#include "cinder/Rect.h"
#include "cinder/DataSource.h"

typedef boost::shared_ptr<class FollowablePath> FollowablePathRef;

class FollowablePath
{
    void ensureCapacity(int minCapacity);
    void read(ci::IStreamRef in);
    
public:
    struct Value
    {
        float x;
        float y;
        float angle;
        float position;
    };
    
    enum
    {
        MODE_BOUNDED,
        MODE_LOOP,
        MODE_TANGENT,
        MODE_MODULO,
    };

    int size;
    int capacity;
    int mode;
    float *x;
    float *y;
    float *len;
    
    FollowablePath(int mode = MODE_TANGENT, int capacity = 256);
    FollowablePath(ci::DataSourceRef source, int mode = MODE_TANGENT);
    
    ~FollowablePath();
    
    void clear();
    float getLength();
    void add(float x, float y);
    
    Value pos2Value(float pos) const;
    ci::Vec2f pos2Point(float pos) const;
    float pos2Angle(float pos) const;
    float pos2SampledAngle(float pos, float sampleSize) const;
    ci::Vec2f pos2Gradient(float pos, float sampleSize) const;
    
    bool findClosestPoint(float x, float y, float min, float *res);
    void closestPointFromSegment(float x, float y, int segmentIndex, float *res);
    
    ci::Rectf getBounds() const;
};
