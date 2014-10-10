/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderSketchSimple.h"
#include "chronotext/cinder/CinderApp.h"

using namespace std;
using namespace ci;
using namespace ci::app;

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
    
    bool CinderSketchSimple::isEmulated() const
    {
        return static_cast<CinderApp*>(context)->isEmulated();
    }
    
    DisplayInfo CinderSketchSimple::getDisplayInfo() const
    {
        return static_cast<CinderApp*>(context)->getDisplayInfo();
    }
    
    WindowInfo CinderSketchSimple::getWindowInfo() const
    {
        return static_cast<CinderApp*>(context)->getWindowInfo();
    }
    
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
