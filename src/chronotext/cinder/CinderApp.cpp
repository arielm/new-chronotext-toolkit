/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderApp.h"
#include "chronotext/system/SystemInfo.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chronotext
{
    CinderApp::CinderApp()
    :
    AppNative(),
    emulated(false),
    startCount(0),
    updateCount(0)
    {}

    void CinderApp::setup()
    {
        updateDisplayAndWindowInfo();
        
        /*
         * App::privateUpdate__ HACKING: SEE COMMENT IN CinderApp::update
         */
        io_service().post([this]{ sketch->clock().update(); });
        
        sketch->setIOService(io_service());
        sketch->timeline().stepTo(0);
        sketch->setup();
    }
    
    void CinderApp::shutdown()
    {
        stop();
        sketch->stop(CinderSketch::FLAG_APP_HIDDEN);
        sketch->shutdown();
        delete sketch;
    }
    
    void CinderApp::resize()
    {
        sketch->resize();
        
        if (startCount == 0)
        {
            start();
            sketch->start(CinderSketch::FLAG_APP_SHOWN);
            startCount++;
        }
    }
    
    void CinderApp::update()
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
         *
         * NOTE THAT getTime() COULD HAVE BEEN ALREADY CALLED
         * WITHIN ONE OF THE PREVIOUSLY "POLLED" FUNCTIONS
         */
        double now = sketch->clock().getTime();
        
        sketch->update();
        sketch->timeline().stepTo(now); // WE CAN'T CONTROL THE APP'S TIMELINE SO WE NEED OUR OWN
        updateCount++;
    }
    
    void CinderApp::draw()
    {
        if (updateCount == 0)
        {
            update(); // HANDLING CASES WHERE draw() IS INVOKED BEFORE update()
        }
        
        sketch->draw();
    }
    
    void CinderApp::mouseDown(MouseEvent event)
    {
        sketch->addTouch(0, event.getX(), event.getY());
    }
    
    void CinderApp::mouseDrag(MouseEvent event)
    {
        sketch->updateTouch(0, event.getX(), event.getY());
    }
    
    void CinderApp::mouseUp(MouseEvent event)
    {
        sketch->removeTouch(0, event.getX(), event.getY());
    }
    
    void CinderApp::touchesBegan(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            sketch->addTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderApp::touchesMoved(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            sketch->updateTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderApp::touchesEnded(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            sketch->removeTouch(touch.getId() - 1, touch.getX(), touch.getY());
        }
    }
    
    void CinderApp::accelerated(AccelEvent event)
    {
        sketch->accelerated(event);
    }
    
    void CinderApp::sendMessageToSketch(int what, const string &body)
    {
        sketch->sendMessage(Message(what, body));
    }
    
    WindowInfo CinderApp::getWindowInfo() const
    {
        return isEmulated() ? emulatedDevice.windowInfo : realWindowInfo;
    }
    
    DisplayInfo CinderApp::getDisplayInfo() const
    {
        return isEmulated() ? emulatedDevice.displayInfo : realDisplayInfo;
    }
    
    bool CinderApp::isEmulated() const
    {
        return emulated;
    }
    
    /*
     * TODO: ADDITIONAL CARE IS REQUIRED FOR "SIMULATED" CONTENT-SCALE AND ANTI-ALIASING
     */
    void CinderApp::emulate(Settings *settings, const EmulatedDevice &device)
    {
        LOGD << "EMULATED DEVICE: " << device << endl;
        
        emulatedDevice = device;
        emulated = true;

        settings->setWindowSize(emulatedDevice.windowInfo.size);

        /*
         * POINTLESS TO ALLOW RESIZE WHEN EMULATING
         * IT WOULD ALSO COMPLICATE EVERYTHING...
         */
        settings->setResizable(false);
    }
    
    void CinderApp::updateDisplayAndWindowInfo()
    {
        updateRealDisplayInfo();
        updateRealWindowInfo();
    }
    
    void CinderApp::updateRealDisplayInfo()
    {
        float contentScale = getWindowContentScale();
        Vec2i baseSize = getWindowSize() / contentScale;
        
        realDisplayInfo = DisplayInfo::create(baseSize.x, baseSize.y, contentScale);
    }
    
    void CinderApp::updateRealWindowInfo()
    {
        realWindowInfo.size = getWindowSize();
        realWindowInfo.aaLevel = DisplayHelper::getAALevel(static_pointer_cast<RendererGl>(getRenderer()));
    }
    
    void CinderApp::start()
    {
        sketch->clock().start();
    }
    
    void CinderApp::stop()
    {
        sketch->clock().stop();
        LOGI << "AVERAGE FRAME-RATE: " << getAverageFps() << " FPS" << endl;
    }
}
