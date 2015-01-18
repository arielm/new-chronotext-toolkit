/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderSketchBase.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    double CinderSketchBase::getElapsedSeconds() const
    {
        return timer.getSeconds(); // OUR FrameClock IS NOT SUITED BECAUSE IT PROVIDES A UNIQUE TIME-SAMPLE PER FRAME
    }
    
    int CinderSketchBase::getElapsedFrames() const
    {
        return frameCount;
    }
    
    const WindowInfo& CinderSketchBase::getWindowInfo() const
    {
        return windowInfo;
    }
    
    FrameClock::Ref CinderSketchBase::clock() const
    {
        return _clock;
    }
    
    ci::Timeline& CinderSketchBase::timeline() const
    {
        return *_timeline;
    }
    
    // ---

    void CinderSketchBase::performSetup(const WindowInfo &windowInfo)
    {
        this->windowInfo = windowInfo;
        forceResize = true;
        
        _timeline->stepTo(0);
        setup();
    }
    
    void CinderSketchBase::performResize(const Vec2i &size)
    {
        if (forceResize || (size != windowInfo.size))
        {
            windowInfo.size = size;
            forceResize = false;
            
            resize();
        }
    }
    
    void CinderSketchBase::performStart(Reason reason)
    {
        frameCount = 0;
        
        timer.start();
        _clock->start();
        
        start(reason);
    }
    
    void CinderSketchBase::performStop(Reason reason)
    {
        timer.stop();
        _clock->stop();
        
        stop(reason);
    }
    
    void CinderSketchBase::performUpdate()
    {
        double now = _clock->getTime();
        
        update();
        _timeline->stepTo(now);
        
        frameCount++;
    }
}
