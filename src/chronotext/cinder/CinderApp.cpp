#include "chronotext/cinder/CinderApp.h"
#include "chronotext/utils/Utils.h"

using namespace ci;
using namespace app;
using namespace std;

void CinderApp::start()
{
    ticks = 0;
}

void CinderApp::stop()
{
    LOGI << "AVERAGE FRAME-RATE: " << ticks / elapsed << " FPS" << endl;
}

void CinderApp::setup()
{
    sketch->setup(false);
    
#if defined(CINDER_COCOA_TOUCH)
    getSignalDidBecomeActive().connect(bind(&CinderApp::start, this));
    getSignalWillResignActive().connect(bind(&CinderApp::stop, this));
#endif
}

void CinderApp::shutdown()
{
    stop(); // XXX: CURRENTLY ONLY USED FOR FPS-COUNTER
    sketch->stop(CinderSketch::FLAG_FOCUS_LOST);
    sketch->shutdown();
    delete sketch;
}

void CinderApp::resize()
{
    sketch->resize();

    if (startCount == 0)
    {
        start(); // XXX: CURRENTLY ONLY USED FOR FPS-COUNTER
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
    
    sketch->run(); // NECESSARY FOR THE "MESSAGE-PUMP"
    sketch->update();
    updateCount++;
}

void CinderApp::draw()
{
    if (updateCount == 0)
    {
        update();
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

#if defined(CINDER_ANDROID)
void CinderApp::resume(bool renewContext)
{
    if (renewContext)
    {
        sketch->setup(true);
    }
    
    sketch->start(CinderSketch::FLAG_APP_RESUME);
}

void CinderApp::pause()
{
    sketch->stop(CinderSketch::FLAG_APP_PAUSE);
}
#endif

void CinderApp::sendMessageToSketch(int what, const string &body)
{
    sketch->sendMessage(Message(what, body));
}
