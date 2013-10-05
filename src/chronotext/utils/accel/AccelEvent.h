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
