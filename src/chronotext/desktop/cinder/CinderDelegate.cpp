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
    CinderSketch* CinderDelegate::getSketch()
    {
        return sketch;
    }
    
    /*
     * XXX: SHOULD NOT BE INVOKED BEFORE THE UNDERLYING WINDOW HAS BEEN CREATED
     *
     * OTHERWISE: POTENTIAL EXTENDERS LIKE osx/cinder/CinderBridge WILL CRASH
     */
    
    void CinderDelegate::setSketch(CinderSketch *newSketch)
    {
        if (sketch)
        {
            destroySketch(sketch);
            sketchDestroyed(sketch);
            sketch = nullptr;
        }
        
        if (newSketch)
        {
            sketch = newSketch;
            sketch->setDelegate(this);
            sketchCreated(sketch);
        }
    }

    // ---
    
    void CinderDelegate::setup()
    {
        /*
         * TODO: TEST ON RETINA DISPLAY (OSX)
         */
        
        float contentScale = getWindowContentScale();
        Vec2i baseSize = getWindowSize() / contentScale;
        
        initInfo.actualDisplayInfo = DisplayInfo::create(baseSize.x, baseSize.y, contentScale);
        
        // ---
        
        INTERN::init(initInfo);
        
        setSketch(createSketch());
        sketch->init();
        
        // ---
        
        INTERN::launch(system::LaunchInfo(io_service()));
        sketch->launch();

        // ---
        
        windowInfo = WindowInfo(getWindowSize(), DisplayHelper::getAALevel(this));
        INTERN::setup(system::SetupInfo(windowInfo));

        /*
         * App::privateUpdate__ HACKING: SEE COMMENT IN CinderDelegate::update()
         */
        io_service().post([this]{ sketch->clock()->update(); });
        
        sketch->timeline().stepTo(0);
        sketch->setup();
    }
    
    void CinderDelegate::shutdown()
    {
        stop(); // NOT HAPPENING AUTOMATICALLY UPON SHUT-DOWN (I.E. UNLIKE ON MOBILE PLATFORMS)
        
        sketch->shutdown();
        setSketch(nullptr);
        
        /*
         * TODO:
         *
         * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
         * - SEE RELATED TODOS IN Context AND TaskManager
         */
        INTERN::shutdown();
    }
    
    void CinderDelegate::resize()
    {
        /*
         * RESIZING IS NOT SUPPORTED WHEN EMULATING
         */
        assert(!(initInfo.emulated && (startCount > 0)));
        
        windowInfo.size = getWindowSize();
        sketch->resize();
        
        if (startCount == 0)
        {
            start();
            startCount++;
        }
    }
    
    void CinderDelegate::update()
    {
        /*
         * App::privateUpdate__ HACKING:
         * WE MUST UPDATE THE CLOCK AT THE BEGINNING OF THE FRAME,
         * AND WE NEED THIS TO TAKE PLACE BEFORE THE FUNCTIONS
         * "POSTED" DURING CinderSketch::update ARE "POLLED"
         */
        io_service().post([this]{ sketch->clock()->update(); });

        /*
         * MUST BE CALLED BEFORE Sketch::update
         * ANY SUBSEQUENT CALL WILL RETURN THE SAME TIME-VALUE
         */
        double now = sketch->clock()->getTime();
        
        sketch->update();
        sketch->timeline().stepTo(now); // WE CAN'T CONTROL THE APP'S TIMELINE SO WE NEED OUR OWN
        
        updateCount++;
    }
    
    void CinderDelegate::draw()
    {
        if (updateCount == 0)
        {
            update(); // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
        }
        
        sketch->draw();
    }
    
    bool CinderDelegate::isEmulated() const
    {
        return initInfo.emulated;
    }
    
    const WindowInfo& CinderDelegate::getWindowInfo() const
    {
        return initInfo.emulated ? initInfo.emulatedDevice.windowInfo : windowInfo;
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
                    sketch->event(CinderSketch::EVENT_BACK);
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
                    sketch->event(CinderSketch::EVENT_ESCAPE);
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
    
#pragma mark ---------------------------------------- SKETCH <-> DELEGATE COMMUNICATION ----------------------------------------
    
    bool CinderDelegate::handleAction(int actionId)
    {
        switch (actionId)
        {
            case CinderSketch::ACTION_CAPTURE_BACK:
                backCaptured = true;
                return true;
                
            case CinderSketch::ACTION_RELEASE_BACK:
                backCaptured = false;
                return true;
                
            case CinderSketch::ACTION_CAPTURE_ESCAPE:
                escapeCaptured = true;
                return true;
                
            case CinderSketch::ACTION_RELEASE_ESCAPE:
                escapeCaptured = false;
                return true;
        }
        
        return false;
    }

    void CinderDelegate::sendMessageToSketch(int what, const string &body)
    {
        sketch->sendMessage(Message(what, body));
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
    
    // ---
    
    void CinderDelegate::start()
    {
        sketch->clock()->start();
        sketch->start(CinderSketch::REASON_APP_SHOWN);
    }
    
    void CinderDelegate::stop()
    {
        sketch->clock()->stop();
        sketch->stop(CinderSketch::REASON_APP_HIDDEN);
        
        LOGI << "AVERAGE FRAME-RATE: " << getAverageFps() << " FPS" << endl;
    }
}
