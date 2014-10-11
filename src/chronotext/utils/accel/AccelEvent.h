/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * BASED ON AccelEvent CLASS FROM CINDER 0.8.4
 * https://github.com/cinder/Cinder/blob/ed921470e92117d1e5b06d97bc070ec12462c3b5/include/cinder/app/AccelEvent.h
 */

#pragma once

#include "cinder/Vector.h"

class AccelEvent
{
public:
    AccelEvent(const ci::Vec3f &data, const ci::Vec3f &rawData, const ci::Vec3f &prevData, const ci::Vec3f &prevRawData)
    :
    data(data),
    rawData(rawData),
    prevData(prevData),
    prevRawData(prevRawData)
    {}
    
    ci::Vec3f getData() const { return data; }
    ci::Vec3f getRawData() const { return rawData; }
    ci::Vec3f getPrevData() const { return prevData; }
    ci::Vec3f getPrevRawData() const { return prevRawData; }
    
    bool isShaking(float shakeDelta = 2.2f) const
    {
        return (rawData - prevRawData).length() > shakeDelta;
    }
    
    class Filter
    {
    public:
        Filter()
        :
        factor(0.1f),
        lastAccel(ci::Vec3f::zero()),
        lastRawAccel(ci::Vec3f::zero())
        {}
        
        Filter(float factor)
        :
        factor(factor),
        lastAccel(ci::Vec3f::zero()),
        lastRawAccel(ci::Vec3f::zero())
        {}
        
        AccelEvent process(const ci::Vec3f &acceleration)
        {
            ci::Vec3f filtered = lastAccel * (1 - factor) + acceleration * factor;
            AccelEvent event(filtered, acceleration, lastAccel, lastRawAccel);
            
            lastAccel = filtered;
            lastRawAccel = acceleration;
            
            return event;
        }
        
    protected:
        float factor;
        ci::Vec3f lastAccel, lastRawAccel;
    };
    
protected:
    ci::Vec3f data, prevData;
	ci::Vec3f rawData, prevRawData;
};
