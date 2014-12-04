/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/app/cinder/CinderDelegate.h"
#include "chronotext/system/Context.h"
#include "chronotext/utils/Utils.h"

#include "cinder/Json.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chronotext
{
    CinderDelegate::CinderDelegate()
    :
    AppNative(),
    startCount(0),
    updateCount(0),
    sketch(nullptr),
    emulated(false)
    {}
    
    CinderSketch* CinderDelegate::getSketch()
    {
        return sketch;
    }
    
    /*
     * XXX: SHOULD NOT BE INVOKED BEFORE THE UNDERLYING WINDOW HAS BEEN CREATED
     *
     * OTHERWISE: POTENTIAL EXTENDERS LIKE osx/cinder/CinderAdapter WILL CRASH
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
    
    void CinderDelegate::sendMessageToSketch(int what, const string &body)
    {
        sketch->sendMessage(Message(what, body));
    }
    
    // ---
    
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
    
    void CinderDelegate::setup()
    {
        updateRealDisplayInfo();
        updateRealWindowInfo();

        context::init(); // TODO: HANDLE FAILURE
        
        setSketch(createSketch());
        sketch->init(); // TODO: HANDLE FAILURE
        
        // ---

        context::setup(io_service());

        /*
         * App::privateUpdate__ HACKING: SEE COMMENT IN CinderDelegate::update
         */
        io_service().post([this]{ sketch->clock().update(); });
        
        sketch->timeline().stepTo(0);
        sketch->setup();
    }
    
    void CinderDelegate::shutdown()
    {
        stop();
        
        sketch->shutdown();
        setSketch(nullptr);
        
        /*
         * TODO:
         *
         * PROPERLY HANDLE THE SHUTING-DOWN OF "UNDERGOING" TASKS
         * RELATED TODOS IN Context AND TaskManager
         */
        
        context::shutdown();
    }
    
    void CinderDelegate::resize()
    {
        /*
         * RESIZING IS NOT SUPPORTED WHEN EMULATING
         */
        assert(!(emulated && (startCount > 0)));
        
        realWindowInfo.size = getWindowSize();
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
        io_service().post([this]{ sketch->clock().update(); });
        
        /*
         * MUST BE CALLED BEFORE Sketch::update
         * ANY SUBSEQUENT CALL WILL RETURN THE SAME TIME-VALUE
         */
        double now = sketch->clock().getTime();
        
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
        return emulated;
    }
    
    WindowInfo CinderDelegate::windowInfo() const
    {
        return emulated ? emulatedDevice.windowInfo : realWindowInfo;
    }
    
    DisplayInfo CinderDelegate::displayInfo() const
    {
        return emulated ? emulatedDevice.displayInfo : realDisplayInfo;
    }
    
#pragma mark ---------------------------------------- INPUT ----------------------------------------
    
    void CinderDelegate::accelerated(AccelEvent event)
    {
        sketch->accelerated(event);
    }
    
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
    
#pragma mark ---------------------------------------- EMULATION ----------------------------------------
    
    /*
     * TODO: ADDITIONAL CARE IS REQUIRED FOR "SIMULATED" CONTENT-SCALE AND ANTI-ALIASING
     */
    
    void CinderDelegate::emulate(Settings *settings, EmulatedDevice &device, DisplayInfo::Orientation orientation)
    {
        emulated = true;
        emulatedDevice = device; // COPYING, IN ORDER TO ALLOW ROTATION
        
        if (device.displayInfo.getOrientation() != orientation)
        {
            emulatedDevice.rotate();
        }
        
        settings->setWindowSize(emulatedDevice.windowInfo.size);
        
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
    
    bool CinderDelegate::loadEmulators(InputSourceRef source)
    {
        emulators.clear();
        
        JsonTree doc(source->loadDataSource());
        
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
    
#pragma mark ---------------------------------------- MISC ----------------------------------------
    
    void CinderDelegate::updateRealDisplayInfo()
    {
        float contentScale = getWindowContentScale();
        Vec2i baseSize = getWindowSize() / contentScale;
        
        realDisplayInfo = DisplayInfo::create(baseSize.x, baseSize.y, contentScale);
    }
    
    void CinderDelegate::updateRealWindowInfo()
    {
        realWindowInfo = WindowInfo(getWindowSize(), DisplayHelper::getAALevel(this));
    }
    
    void CinderDelegate::start()
    {
        sketch->clock().start();
        sketch->start(CinderSketch::FLAG_APP_SHOWN);
    }
    
    void CinderDelegate::stop()
    {
        sketch->clock().stop();
        sketch->stop(CinderSketch::FLAG_APP_HIDDEN);
        
        LOGI << "AVERAGE FRAME-RATE: " << getAverageFps() << " FPS" << endl;
    }
}
