/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if !defined(CINDER_MAC)
#   error UNSUPPORTED PLATFORM
#endif

// ---

#include "chronotext/cinder/CinderDelegateBase.h"
#include "chronotext/InputSource.h"

#include "cinder/app/AppNative.h"

namespace chr
{
    class CinderDelegate : public CinderDelegateBase, public ci::app::AppNative
    {
    public:
        void prepareSettings(Settings *settings) final;
        void setup() final;
        void shutdown() final;
        void resize() final;
        void update() final;
        void draw() final;

        void mouseDown(ci::app::MouseEvent event) final;
        void mouseUp(ci::app::MouseEvent event) final;
        void mouseDrag(ci::app::MouseEvent event) final;
        
        void touchesBegan(ci::app::TouchEvent event) final;
        void touchesMoved(ci::app::TouchEvent event) final;
        void touchesEnded(ci::app::TouchEvent event) final;
        
        void keyDown(ci::app::KeyEvent event) final;
        void keyUp(ci::app::KeyEvent event) final;
        
        void sendMessageToBridge(int what, const std::string &body = "") final;
        void performAction(int actionId) final;
        
        virtual void applySettings(Settings *settings) {}

        void emulate(Settings *settings, EmulatedDevice &device, DisplayInfo::Orientation orientation = DisplayInfo::ORIENTATION_DEFAULT);
        bool emulate(Settings *settings, const std::string &deviceKey, DisplayInfo::Orientation orientation = DisplayInfo::ORIENTATION_DEFAULT);
        bool loadEmulators(chr::InputSource::Ref inputSource);

    protected:
        bool prepared = false;
        
        int updateCount = 0;
        int resizeCount = 0;

        bool backCaptured = false;
        bool escapeCaptured = false;
        
        std::map<std::string, std::shared_ptr<EmulatedDevice>> emulators;
    };
}
