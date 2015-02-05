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
    
    bool CinderDelegate::init()
    {
        intern::instance = this;
        
        return _init();
    }
    
    void CinderDelegate::launch()
    {
        startIOService();
        
        launchInfo.io_service = io.get();
        _launch();
    }
    
    void CinderDelegate::setup(const WindowInfo &windowInfo)
    {
        setupInfo.windowInfo = windowInfo;
        _setup();
    }
    
    void CinderDelegate::shutdown()
    {
        /*
         * TODO:
         *
         * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
         * - SEE RELATED TODOS IN CinderDelegateBase AND TaskManager
         */
        _shutdown();
        
        stopIOService();
        
        intern::instance = nullptr;
    }
    
    void CinderDelegate::resize(const ci::Vec2i &size)
    {
        setupInfo.windowInfo.size = size;
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
    
    void CinderDelegate::handleEvent(int eventId)
    {
        switch (eventId)
        {
            case CinderSketch::EVENT_RESUMED:
                sketch->performStart(CinderSketch::START_REASON_APP_RESUMED);
                return;
                
            case CinderSketch::EVENT_SHOWN:
                sketch->performStart(CinderSketch::START_REASON_VIEW_SHOWN);
                return;
                
            case CinderSketch::EVENT_PAUSED:
                sketch->performStop(CinderSketch::STOP_REASON_APP_PAUSED);
                return;
                
            case CinderSketch::EVENT_HIDDEN:
                sketch->performStop(CinderSketch::STOP_REASON_VIEW_HIDDEN);
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
    
#pragma mark ---------------------------------------- KEYBOARD ----------------------------------------

    int CinderDelegateBase::getCode(const KeyEvent &keyEvent)
    {
        return 0;
    }
    
    bool CinderDelegateBase::isShiftDown(const KeyEvent &keyEvent)
    {
        return false;
    }
    
    bool CinderDelegateBase::isAltDown(const KeyEvent &keyEvent)
    {
        return false;
    }
    
    bool CinderDelegateBase::isAccelDown(const KeyEvent &keyEvent)
    {
        return false;
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
        UIAccelerometer.sharedAccelerometer.delegate = system::bridge;
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