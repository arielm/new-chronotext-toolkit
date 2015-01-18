/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/ios/cinder/CinderDelegate.h"
#include "chronotext/Context.h"

#import "chronotext/ios/cinder/CinderBridge.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    bool CinderDelegate::init()
    {
        INTERN::delegate = this;
        INTERN::init(system::InitInfo());
        
        sketch = createSketch();
        sketchCreated(sketch);
        sketch->init();
        
        return true;
    }
    
    void CinderDelegate::launch()
    {
        startIOService();
        
        INTERN::launch(system::LaunchInfo(*io));
        sketch->launch();
    }
    
    void CinderDelegate::setup(const WindowInfo &windowInfo)
    {
        INTERN::setup(system::SetupInfo(windowInfo));
        sketch->performSetup(windowInfo);
    }
    
    void CinderDelegate::shutdown()
    {
        sketch->shutdown();
        delete sketch;
        sketchDestroyed(sketch);
        sketch = nullptr;
        
        /*
         * TODO:
         *
         * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
         * - SEE RELATED TODOS IN Context AND TaskManager
         */
        
        INTERN::shutdown();
        INTERN::delegate = nullptr;
        
        stopIOService();
    }
    
    void CinderDelegate::resize(const ci::Vec2f &size)
    {
        sketch->performResize(size);
    }
    
    void CinderDelegate::update()
    {
        /*
         * SHOULD TAKE PLACE BEFORE IO-SERVICE-POLLING
         *
         * SUBSEQUENT CALLS TO FrameClock::getTime() DURING THE FRAME WILL RETURN THE SAME TIME-SAMPLE
         */
        sketch->clock()->update(true);
        
        io->poll();
        
        sketch->performUpdate();
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
    
#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------
    
    void CinderDelegate::messageFromBridge(int what, const std::string &body)
    {
        sketch->sendMessage(Message(what, body));
    }
    
    void CinderDelegate::handleEvent(int eventId)
    {
        switch (eventId)
        {
            case CinderSketch::EVENT_RESUMED:
                sketch->performStart(CinderSketch::REASON_APP_RESUMED);
                return;
                
            case CinderSketch::EVENT_SHOWN:
                sketch->performStart(CinderSketch::REASON_APP_SHOWN);
                return;
                
            case CinderSketch::EVENT_PAUSED:
                sketch->performStop(CinderSketch::REASON_APP_PAUSED);
                return;
                
            case CinderSketch::EVENT_HIDDEN:
                sketch->performStop(CinderSketch::REASON_APP_HIDDEN);
                return;
        }
        
        sketch->event(eventId);
    }
    
#pragma mark ---------------------------------------- IO-SERVICE ----------------------------------------
    
    void CinderDelegate::startIOService()
    {
        io = make_shared<boost::asio::io_service>();
        ioWork = make_shared<boost::asio::io_service::work>(*io);
    }
    
    void CinderDelegate::stopIOService()
    {
        io->stop();
    }
    
#pragma mark ---------------------------------------- TOUCH ----------------------------------------
    
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
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    void CinderDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        accelFilter = AccelEvent::Filter(filterFactor);
        
        if (updateFrequency <= 0)
        {
            updateFrequency = 30;
        }
        
        UIAccelerometer.sharedAccelerometer.updateInterval = 1 / updateFrequency;
        UIAccelerometer.sharedAccelerometer.delegate = INTERN::bridge;
    }
    
    void CinderDelegate::disableAccelerometer()
    {
        UIAccelerometer.sharedAccelerometer.delegate = nil;
    }
    
    void CinderDelegate::handleAcceleration(const Vec3f &acceleration)
    {
        sketch->accelerated(accelFilter.process(acceleration));
    }
}