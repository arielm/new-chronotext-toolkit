/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/app/cinder/CinderSketchSimple.h"
#include "chronotext/app/cinder/CinderApp.h"

using namespace std;
using namespace ci;
using namespace ci::app;

#define NATIVE_CONTEXT static_cast<CinderApp*>(context)

namespace chronotext
{
    CinderSketchSimple::CinderSketchSimple(void *context, void *delegate)
    :
    CinderSketchBase(),
    context(static_cast<AppNative*>(context)),
    delegate(static_cast<CinderApp*>(delegate)),
    mClock(new FrameClock()),
    mTimeline(Timeline::create())
    {}
    
#pragma mark ---------------------------------------- GETTERS ----------------------------------------

    bool CinderSketchSimple::isEmulated() const
    {
        return NATIVE_CONTEXT->isEmulated();
    }
    
    DisplayInfo CinderSketchSimple::getDisplayInfo() const
    {
        return NATIVE_CONTEXT->getDisplayInfo();
    }
    
    WindowInfo CinderSketchSimple::getWindowInfo() const
    {
        return NATIVE_CONTEXT->getWindowInfo();
    }
    
#pragma mark ---------------------------------------- ACTIONS AND MESSAGES ----------------------------------------
    
    void CinderSketchSimple::action(int actionId)
    {
        if (delegate)
        {
            delegate->action(actionId);
        }
    }
    
    void CinderSketchSimple::sendMessageToDelegate(int what, const string &body)
    {
        if (delegate)
        {
            delegate->receiveMessageFromSketch(what, body);
        }
    }
}
