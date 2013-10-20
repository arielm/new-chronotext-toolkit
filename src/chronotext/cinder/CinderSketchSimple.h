/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/app/AppNative.h"

#include "chronotext/cinder/CinderSketchBase.h"

namespace chronotext
{
    class CinderApp;
    
    class CinderSketchSimple : public CinderSketchBase
    {
    public:
        CinderSketchSimple(void *context, void *delegate = NULL);
        
        std::ostream& console() { return context->console(); }
        boost::asio::io_service& io_service() const { return context->io_service(); }
        
        double getElapsedSeconds() const { return context->getElapsedSeconds(); }
        uint32_t getElapsedFrames() const { return context->getElapsedFrames(); }
        
        int getWindowWidth() const { return context->getWindowWidth(); }
        int getWindowHeight() const { return context->getWindowHeight(); }
        ci::Vec2f getWindowCenter() const { return context->getWindowCenter(); }
        ci::Vec2i getWindowSize() const { return context->getWindowSize(); }
        float getWindowAspectRatio() const { return context->getWindowAspectRatio(); }
        ci::Area getWindowBounds() const { return context->getWindowBounds(); }
        float getWindowContentScale() const { return context->getWindowContentScale(); }
        
        void sendMessageToDelegate(int what, const std::string &body = "");
        
    protected:
        ci::app::AppNative *context;
        CinderApp *delegate;
    };
}

namespace chr = chronotext;
