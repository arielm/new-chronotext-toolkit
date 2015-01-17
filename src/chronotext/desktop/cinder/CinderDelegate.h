/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderDelegateBase.h"
#include "chronotext/system/SystemInfo.h"
#include "chronotext/InputSource.h"

#include "cinder/app/AppNative.h"

namespace chr
{
    class CinderDelegate : public CinderDelegateBase, public ci::app::AppNative
    {
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> LOG_WARNING;
        
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
        
        void messageFromBridge(int what, const std::string &body = "") final;
        void performAction(int actionId) final;
        
        const WindowInfo& getWindowInfo() const final;
        double elapsedSeconds() const;
        int elapsedFrames() const;
        
        virtual void applySettings(Settings *settings) {}
        virtual void applyDefaultSettings(Settings *settings);
        void prepareSettings(Settings *settings) final;

        void emulate(Settings *settings, EmulatedDevice &device, DisplayInfo::Orientation orientation = DisplayInfo::ORIENTATION_DEFAULT);
        bool emulate(Settings *settings, const std::string &deviceKey, DisplayInfo::Orientation orientation = DisplayInfo::ORIENTATION_DEFAULT);
        bool loadEmulators(chr::InputSource::Ref inputSource);
        
        int getCode(const ci::app::KeyEvent &keyEvent) const final;
        bool isShiftDown(const ci::app::KeyEvent &keyEvent) const final;
        bool isAltDown(const ci::app::KeyEvent &keyEvent) const final;
        bool isAccelDown(const ci::app::KeyEvent &keyEvent) const final;

    protected:
        CinderSketch *sketch = nullptr;
        
        ci::Timer timer;
        int frameCount = 0;
        int resizeCount = 0;

        system::InitInfo initInfo;
        WindowInfo windowInfo;

        bool backCaptured = false;
        bool escapeCaptured = false;
        
        std::map<std::string, std::shared_ptr<EmulatedDevice>> emulators;
       
        void start(CinderSketch::Reason reason);
        void stop(CinderSketch::Reason reason);
    };
}
