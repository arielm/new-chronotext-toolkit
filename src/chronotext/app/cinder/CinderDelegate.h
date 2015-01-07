/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/system/SystemInfo.h"
#include "chronotext/InputSource.h"

#include "cinder/app/AppNative.h"

namespace chr
{
    class CinderDelegate : public ci::app::AppNative
    {
    public:
        CinderSketch* getSketch();

        virtual void sketchCreated(CinderSketch *sketch) {}
        virtual void sketchDestroyed(CinderSketch *sketch) {}

        virtual bool handleAction(int actionId);
        virtual void sendMessageToSketch(int what, const std::string &body = "");
        virtual void receiveMessageFromSketch(int what, const std::string &body) {}

        virtual void applyDefaultSettings(Settings *settings);
        virtual void applySettings(Settings *settings) {}
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
        
        bool isEmulated() const;
        const WindowInfo& getWindowInfo() const;
        
        void emulate(Settings *settings, EmulatedDevice &device, DisplayInfo::Orientation orientation = DisplayInfo::ORIENTATION_DEFAULT);
        bool emulate(Settings *settings, const std::string &deviceKey, DisplayInfo::Orientation orientation = DisplayInfo::ORIENTATION_DEFAULT);
        bool loadEmulators(chr::InputSource::Ref inputSource);
        
    protected:
        CinderSketch *sketch = nullptr;
        
        int startCount = 0;
        int updateCount = 0;
        
        bool backCaptured = false;
        bool escapeCaptured = false;

        system::InitInfo initInfo;
        WindowInfo windowInfo;
        
        std::map<std::string, std::shared_ptr<EmulatedDevice>> emulators;
        
        void setSketch(CinderSketch *sketch);
       
        void start();
        void stop();
    };
}
