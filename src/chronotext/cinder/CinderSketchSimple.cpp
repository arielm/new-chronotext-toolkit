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
using namespace app;

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
    
    WindowInfo CinderSketchSimple::getWindowInfo() const
    {
        WindowInfo info;
        
        info.size = getWindowSize(),
        info.contentScale = getWindowContentScale(),
        
        /*
         * TODO
         */
        info.diagonal = 0;
        info.density=  0;
        
        switch (static_pointer_cast<RendererGl>(context->getRenderer())->getAntiAliasing())
        {
            case RendererGl::AA_NONE:
                info.aaLevel = 0;
                break;
                
            case RendererGl::AA_MSAA_2:
                info.aaLevel = 2;
                break;
                
            case RendererGl::AA_MSAA_4:
                info.aaLevel = 4;
                break;
                
            case RendererGl::AA_MSAA_6:
                info.aaLevel = 6;
                break;
                
            case RendererGl::AA_MSAA_8:
                info.aaLevel = 8;
                break;
                
            case RendererGl::AA_MSAA_16:
                info.aaLevel = 16;
                break;
                
            case RendererGl::AA_MSAA_32:
                info.aaLevel = 32;
                break;
        }
        
        return info;
    }
    
    void CinderSketchSimple::sendMessageToDelegate(int what, const string &body)
    {
        if (delegate)
        {
            delegate->receiveMessageFromSketch(what, body);
        }
    }
}
