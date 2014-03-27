/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderApp.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;
using namespace app;

namespace chronotext
{
    CinderApp::CinderApp()
    :
    startCount(0),
    updateCount(0),
    ticks(0)
    {}

    void CinderApp::setup()
    {
        /*
         * App::privateUpdate__ HACKING: SEE COMMENT IN CinderApp::update
         */
        io_service().post([this]{ sketch->clock().update(); });
        
        sketch->setIOService(io_service());
        sketch->timeline().stepTo(0);
        sketch->setup(false);
        
#if defined(CINDER_COCOA_TOUCH)
        getSignalDidBecomeActive().connect(bind(&CinderApp::start, this));
        getSignalWillResignActive().connect(bind(&CinderApp::stop, this));
#endif
    }
    
    void CinderApp::shutdown()
    {
        stop();
        sketch->stop(CinderSketch::FLAG_FOCUS_LOST);
        sketch->shutdown();
        delete sketch;
    }
    
    void CinderApp::resize()
    {
        sketch->resize();
        
        if (startCount == 0)
        {
            start();
            sketch->start(CinderSketch::FLAG_FOCUS_GAINED);
            startCount++;
        }
    }
    
    void CinderApp::update()
    {
        double now = getElapsedSeconds();
        
        if (ticks == 0)
        {
            t0 = now;
        }
        
        ticks++;
        elapsed = now - t0;
        
        // ---
       
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
        now = sketch->clock().getTime();
        
        sketch->update();
        sketch->timeline().stepTo(now); // WE OBVIOUSLY CAN'T USE THE APP'S TIMELINE...
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
        for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
        {
            sketch->addTouch(touchIt->getId() - 1, touchIt->getPos().x, touchIt->getPos().y);
        }
    }
    
    void CinderApp::touchesMoved(TouchEvent event)
    {
        for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
        {
            sketch->updateTouch(touchIt->getId() - 1, touchIt->getPos().x, touchIt->getPos().y);
        }
    }
    
    void CinderApp::touchesEnded(TouchEvent event)
    {
        for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
        {
            sketch->removeTouch(touchIt->getId() - 1, touchIt->getPos().x, touchIt->getPos().y);
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
    
    void CinderApp::emulate(Settings *settings, const EmulatedDevice &device)
    {
        settings->setWindowSize(device.size);

        /*
         * HIGH-DENSITY AND ANTI-ALIASING ARE NOT HANDLED AT THIS LEVEL.
         *
         * HIGH-DENSITY IS ENABLED IF THE SCREEN IS RETINA, BUT CAN ALSO BE TESTED ON A REGULAR SCREEN:
         * https://forum.libcinder.org/topic/rfc-retina-high-density-display-support
         *
         * ANTI-ALIASING SHOULD BE DEFINED VIA THE "CINDER_APP_NATIVE" MACRO
         */
        
        WindowInfo windowInfo;
        windowInfo.size = device.size;
        windowInfo.diagonal = device.diagonal;
        windowInfo.density = (device.diagonal == 0) ? 0: (device.size.length() / device.diagonal);
        
        SystemInfo::instance().setWindowInfo(windowInfo);
    }
    
#if defined(CINDER_ANDROID)
    
    void CinderApp::resume(bool renewContext)
    {
        sketch->setup(true);
        sketch->start(CinderSketch::FLAG_APP_RESUMED);
    }
    
    void CinderApp::pause()
    {
        sketch->event(CinderSketch::EVENT_CONTEXT_LOST);
        sketch->stop(CinderSketch::FLAG_APP_PAUSED);
    }
    
#endif
    
    void CinderApp::start()
    {
        ticks = 0;
        sketch->clock().start();
    }
    
    void CinderApp::stop()
    {
        sketch->clock().stop();
        LOGI << "AVERAGE FRAME-RATE: " << ticks / elapsed << " FPS" << endl;
    }
}
