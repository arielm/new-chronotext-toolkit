/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/desktop/cinder/CinderDelegate.h"
#include "chronotext/Context.h"

#include "cinder/Json.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    atomic<bool> CinderDelegate::LOG_VERBOSE (false);
    atomic<bool> CinderDelegate::LOG_WARNING (true);
    
    namespace intern
    {
        CinderDelegate *instance = nullptr;
    }
    
    CinderDelegate& delegate()
    {
        return checkedReference(intern::instance);
    }
    
    // ---

    void CinderDelegate::setup()
    {
        intern::instance = this;
        
        initInfo.actualContentScale = getWindowContentScale();
        initInfo.actualWindowSize = getWindowSize();
        
        _init();
        
        // ---
        
        launchInfo.io_service = &io_service();
        _launch();
        
        // ---
        
        WindowInfo actualWindowInfo(getWindowSize(), DisplayHelper::getAALevel(this));
        
        setupInfo.windowInfo = initInfo.emulated ? initInfo.emulatedDevice.windowInfo : actualWindowInfo;
        _setup();
    }
    
    void CinderDelegate::shutdown()
    {
        /*
         * NOT HAPPENING "AUTOMATICALLY" (UNLIKE ON MOBILE PLATFORMS)
         */
        sketch->performStop(CinderSketch::STOP_REASON_VIEW_HIDDEN);

        _shutdown();
        
        intern::instance = nullptr;
    }
    
    void CinderDelegate::resize()
    {
        /*
         * RESIZING IS NOT SUPPORTED WHEN EMULATING
         */
        assert((resizeCount == 0) || !initInfo.emulated);
        
        setupInfo.windowInfo.size = initInfo.emulated ? initInfo.emulatedDevice.windowInfo.size : getWindowSize();
        sketch->performResize(setupInfo.windowInfo.size);
        
        /*
         * I.E. THE FIRST AppNative::resize()
         */
        if (resizeCount++ == 0)
        {
            /*
             * NOT HAPPENING "AUTOMATICALLY" (UNLIKE ON MOBILE PLATFORMS)
             */
            sketch->performStart(CinderSketch::START_REASON_VIEW_SHOWN);
            
            /*
             * DESKTOP-ONLY WORKAROUND [1/4]
             *
             * WILL SCHEDULLE TIME-SAMPLING DURING THE NEXT CinderDelegate::update()
             *
             * NECESSARY BECAUSE WE NEED TIME-SAMPLING TO TAKE PLACE BEFORE IO-SERVICE-POLLING,
             * WHICH IS HANDLED WITHIN App::privateUpdate__
             */
            sketch->clock()->update(false);
        }
    }
    
    void CinderDelegate::update()
    {
        /*
         * DESKTOP-ONLY WORKAROUND [2/4]
         *
         * NOTE THAT FrameClock::getTime() MAY HAVE BEEN ALREADY CALLED
         * DURING ONE OF THE FUNCTIONS POLLED WITHIN App::privateUpdate__
         */
        sketch->clock()->getTime();
        
        sketch->performUpdate();
        updateCount++;
    }
    
    void CinderDelegate::draw()
    {
        if (updateCount == 0)
        {
            update(); // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
        }
        
        /*
         * DESKTOP-ONLY WORKAROUND [3/4]
         *
         * NECESSARY BECAUSE CinderDelegate::draw() CAN THEORETICALLY BE INVOKED SEVERAL-TIMES PER FRAME
         */
        sketch->clock()->lock();
        
        sketch->draw();
        
        /*
         * DESKTOP-ONLY WORKAROUND [4/4]
         *
         * WILL RELEASE THE LOCK AND SCHEDULLE TIME-SAMPLING DURING THE NEXT CinderDelegate::update()
         */
        sketch->clock()->update(false);
    }
    
#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------

    void CinderDelegate::performAction(int actionId)
    {
        switch (actionId)
        {
            case CinderSketch::ACTION_CAPTURE_BACK:
                backCaptured = true;
                break;
                
            case CinderSketch::ACTION_RELEASE_BACK:
                backCaptured = false;
                break;
                
            case CinderSketch::ACTION_CAPTURE_ESCAPE:
                escapeCaptured = true;
                break;
                
            case CinderSketch::ACTION_RELEASE_ESCAPE:
                escapeCaptured = false;
                break;
        }
    }
    
#pragma mark ---------------------------------------- LIFE-CYCLE ----------------------------------------
    
    void CinderDelegate::applyDefaultSettings(Settings *settings)
    {
        settings->disableFrameRate(); // WOULD OTHERWISE CAUSE INSTABILITY (IN ANY-CASE: VERTICAL-SYNC IS ALLOWED BY DEFAULT)
        settings->enableHighDensityDisplay();
    }
    
    void CinderDelegate::prepareSettings(Settings *settings)
    {
        applyDefaultSettings(settings);
        applySettings(settings);
    }
    
#pragma mark ---------------------------------------- TOUCH ----------------------------------------
    
    void CinderDelegate::mouseDown(MouseEvent event)
    {
        sketch->addTouch(0, event.getX(), event.getY());
    }
    
    void CinderDelegate::mouseDrag(MouseEvent event)
    {
        sketch->updateTouch(0, event.getX(), event.getY());
    }
    
    void CinderDelegate::mouseUp(MouseEvent event)
    {
        sketch->removeTouch(0, event.getX(), event.getY());
    }
    
    // ---
    
    void CinderDelegate::touchesBegan(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            sketch->addTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderDelegate::touchesMoved(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            sketch->updateTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderDelegate::touchesEnded(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            sketch->removeTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
#pragma mark ---------------------------------------- KEYBOARD ----------------------------------------
    
    void CinderDelegate::keyDown(KeyEvent event)
    {
        switch (event.getCode())
        {
            case KeyEvent::KEY_BACKSPACE:
            {
                if (backCaptured)
                {
                    sketch->event(CinderSketch::EVENT_TRIGGER_BACK);
                }
                else
                {
                    quit();
                }
                
                return;
            }
                
            case KeyEvent::KEY_ESCAPE:
            {
                if (escapeCaptured)
                {
                    sketch->event(CinderSketch::EVENT_TRIGGER_ESCAPE);
                }
                else
                {
                    quit();
                }
                
                return;
            }
        }
        
        sketch->keyDown(event);
    }
    
    void CinderDelegate::keyUp(KeyEvent event)
    {
        sketch->keyUp(event);
    }
    
#pragma mark ---------------------------------------- EMULATION ----------------------------------------
    
    /*
     * XXX: SOME ADDITIONAL CARE IS REQUIRED IN ORDER TO EMULATE CONTENT-SCALE AND ANTI-ALIASING
     */
    
    void CinderDelegate::emulate(Settings *settings, EmulatedDevice &device, DisplayInfo::Orientation orientation)
    {
        initInfo.emulated = true;
        initInfo.emulatedDevice = device;
        
        if (orientation != device.displayInfo.orientation())
        {
            initInfo.emulatedDevice.rotate();
        }
        
        settings->setWindowSize(initInfo.emulatedDevice.windowInfo.size);
        
        /*
         * ALLOWING TO RESIZE AN EMULATOR WOULD BE POINTLESS (AND NOT TRIVIAL TO IMPLEMENT...)
         */
        settings->setResizable(false);
    }
    
    bool CinderDelegate::emulate(Settings *settings, const string &deviceKey, DisplayInfo::Orientation orientation)
    {
        auto it = emulators.find(deviceKey);
        
        if (it != emulators.end())
        {
            emulate(settings, *it->second, orientation);
            return true;
        }
        
        return false;
    }
    
    bool CinderDelegate::loadEmulators(InputSource::Ref inputSource)
    {
        emulators.clear();
        
        JsonTree doc(inputSource->loadDataSource());
        
        for (auto &emulatorNode: doc.getChildren())
        {
            auto emulatorKey = emulatorNode.getKey();
            auto &displayNode = emulatorNode["display"];
            
            // ---
            
            auto &sizeNode = displayNode["size"];
            int displayWidth = sizeNode[0].getValue<int>();
            int displayHeight = sizeNode[1].getValue<int>();
            
            // ---
            
            float contentScale = 1;
            
            if (displayNode.hasChild("contentScale"))
            {
                contentScale = displayNode["contentScale"].getValue<float>();
            }
            
            // ---
            
            DisplayInfo displayInfo;
            
            if (displayNode.hasChild("diagonal"))
            {
                float diagonal = displayNode["diagonal"].getValue<float>();
                displayInfo = DisplayInfo::createWithDiagonal(displayWidth, displayHeight, diagonal, contentScale);
            }
            else if (displayNode.hasChild("density"))
            {
                float density = displayNode["density"].getValue<float>();
                displayInfo = DisplayInfo::createWithDensity(displayWidth, displayHeight, density, contentScale);
            }
            else
            {
                throw EXCEPTION(CinderDelegate, "EMULATOR: " + emulatorKey + ": display MUST HAVE A diagonal OR density");
            }
            
            // ---
            
            if (emulatorNode.hasChild("window"))
            {
                auto &sizeNode = emulatorNode["window"]["size"];
                int windowWidth = sizeNode[0].getValue<int>();
                int windowHeight = sizeNode[1].getValue<int>();
                
                WindowInfo windowInfo(windowWidth, windowHeight);
                
                // ---
                
                emulators[emulatorKey] = make_shared<EmulatedDevice>(displayInfo, windowInfo);
            }
            else
            {
                emulators[emulatorKey] = make_shared<EmulatedDevice>(displayInfo);
            }
        }
        
        return !emulators.empty();
    }
    
#pragma mark ---------------------------------------- TEMPORARY ----------------------------------------

    int CinderDelegate::getCode(const KeyEvent &keyEvent) const
    {
        return keyEvent.getCode();
    }
    
    bool CinderDelegate::isShiftDown(const KeyEvent &keyEvent) const
    {
        return keyEvent.isShiftDown();
    }
    
    bool CinderDelegate::isAltDown(const KeyEvent &keyEvent) const
    {
        return keyEvent.isAltDown();
    }
    
    bool CinderDelegate::isAccelDown(const KeyEvent &keyEvent) const
    {
        return keyEvent.isAccelDown();
    }
}
