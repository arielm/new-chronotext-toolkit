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
        addTouch(0, event.getPos());
    }
    
    void CinderApp::mouseDrag(MouseEvent event)
    {
        updateTouch(0, event.getPos());
    }
    
    void CinderApp::mouseUp(MouseEvent event)
    {
        removeTouch(0, event.getPos());
    }
    
    void CinderApp::touchesBegan(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            addTouch(touch.getId() - 1, touch.getPos());
        }
    }
    
    void CinderApp::touchesMoved(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            updateTouch(touch.getId() - 1, touch.getPos());
        }
    }
    
    void CinderApp::touchesEnded(TouchEvent event)
    {
        for (auto &touch : event.getTouches())
        {
            removeTouch(touch.getId() - 1, touch.getPos());
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
        /*
         * HACK TO OVERCOME MALFUNCTION IN 0.8.5
         * OTHERWISE, WE'D USE settings->getDisplay()->getContentScale()
         */
        stringstream tmp;
        tmp << *settings->getDisplay();
        float realContentScale = (tmp.str().back() == '2') ? 2 : 1;
        
        settings->setWindowSize(device.size * device.contentScale / realContentScale);

        WindowInfo windowInfo;
        windowInfo.size = device.size;
        windowInfo.contentScale = device.contentScale;
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
    
    void CinderApp::addTouch(int index, const Vec2f &position)
    {
        auto scale = sketch->getWindowInfo().contentScale / getWindowContentScale();
        sketch->addTouch(0, position.x / scale, position.y / scale);
    }
    
    void CinderApp::updateTouch(int index, const Vec2f &position)
    {
        auto scale = sketch->getWindowInfo().contentScale / getWindowContentScale();
        sketch->updateTouch(0, position.x / scale, position.y / scale);
    }
    
    void CinderApp::removeTouch(int index, const Vec2f &position)
    {
        auto scale = sketch->getWindowInfo().contentScale / getWindowContentScale();
        sketch->removeTouch(0, position.x / scale, position.y / scale);
    }
}
