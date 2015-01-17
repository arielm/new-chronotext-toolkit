/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderSketch.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    double CinderSketch::getElapsedSeconds() const
    {
        return delegate().elapsedSeconds();
    }
    
    int CinderSketch::getElapsedFrames() const
    {
        return delegate().elapsedFrames();
    }

    bool CinderSketch::isEmulated() const
    {
        return delegate().isEmulated();
    }
    
    const WindowInfo& CinderSketch::getWindowInfo() const
    {
        return delegate().getWindowInfo();
    }
}
