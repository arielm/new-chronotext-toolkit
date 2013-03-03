/*
 * BASED ON AccelEvent CLASS FROM CINDER 0.8.4
 * https://github.com/cinder/Cinder/blob/master/include/cinder/app/AccelEvent.h
 */

#pragma once

#include "cinder/Vector.h"

class AccelEvent
{
    ci::Vec3f mData, mPrevData;
	ci::Vec3f mRawData, mPrevRawData;
    
public:
    AccelEvent(const ci::Vec3f &data, const ci::Vec3f &rawData, const ci::Vec3f &prevData, const ci::Vec3f &prevRawData)
    :
    mData(data),
    mRawData(rawData),
    mPrevData(prevData),
    mPrevRawData(prevRawData)
    {}

    ci::Vec3f getData() const { return mData; }
    ci::Vec3f getRawData() const { return mRawData; }
    ci::Vec3f getPrevData() const { return mPrevData; }
    ci::Vec3f getPrevRawData() const { return mPrevRawData; }
    
    bool isShaking(float shakeDelta = 2.2f) const
    {
        return (mRawData - mPrevRawData).lengthSquared() > shakeDelta * shakeDelta;
    }
};
