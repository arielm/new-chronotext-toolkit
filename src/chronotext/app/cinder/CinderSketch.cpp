/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/app/cinder/CinderSketch.h"
#include "chronotext/app/cinder/CinderDelegate.h"

using namespace std;
using namespace ci;
using namespace ci::app;

#define NATIVE_CONTEXT static_cast<CinderDelegate*>(context)

namespace chronotext
{
    CinderSketch::CinderSketch()
    :
    CinderSketchBase(),
    context(nullptr),
    delegate(nullptr),
    mClock(FrameClock::create()),
    mTimeline(Timeline::create())
    {}
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    bool CinderSketch::isEmulated() const
    {
        return NATIVE_CONTEXT->isEmulated();
    }
    
    DisplayInfo CinderSketch::getDisplayInfo() const
    {
        return NATIVE_CONTEXT->getDisplayInfo();
    }
    
    WindowInfo CinderSketch::getWindowInfo() const
    {
        return NATIVE_CONTEXT->getWindowInfo();
    }
    
#pragma mark ---------------------------------------- ACTIONS AND MESSAGES ----------------------------------------
    
    void CinderSketch::action(int actionId)
    {
        if (delegate)
        {
            delegate->action(actionId);
        }
    }
    
    void CinderSketch::sendMessageToDelegate(int what, const string &body)
    {
        if (delegate)
        {
            delegate->receiveMessageFromSketch(what, body);
        }
    }
}

#undef NATIVE_CONTEXT
