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
    delegate(static_cast<CinderApp*>(delegate))
    {}
    
    float CinderSketchSimple::getWindowDensity() const
    {
        return 0; // TODO, SEE: http://en.wikipedia.org/wiki/List_of_displays_by_pixel_density
    }
    
    int CinderSketchSimple::getWindowAALevel() const
    {
        auto aa = static_pointer_cast<RendererGl>(context->getRenderer())->getAntiAliasing();
        
        switch (aa)
        {
            default:
            case RendererGl::AA_NONE:
                return 0;
                
            case RendererGl::AA_MSAA_2:
                return 2;
                
            case RendererGl::AA_MSAA_4:
                return 4;
                
            case RendererGl::AA_MSAA_6:
                return 6;
                
            case RendererGl::AA_MSAA_8:
                return 8;
                
            case RendererGl::AA_MSAA_16:
                return 16;
                
            case RendererGl::AA_MSAA_32:
                return 32;
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
