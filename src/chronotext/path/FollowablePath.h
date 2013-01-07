#pragma once

#include "cinder/Rect.h"

typedef boost::shared_ptr<class FollowablePath> FollowablePathRef;

class FollowablePath
{
    void ensureCapacity(int minCapacity);
    
public:
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
    ~FollowablePath();
    
    void clear();
    float getLength();
    void add(float x, float y);
    
    void pos2Point(float pos, float *res);
    float pos2Angle(float pos);
    float pos2SampledAngle(float pos, float sampleSize);
    void pos2Gradient(float pos, float sampleSize, float *res);
    
    bool findClosestPoint(float x, float y, float min, float *res);
    void closestPointFromSegment(float x, float y, int segmentIndex, float *res);
    
    ci::Rectf getBounds() const;
};
