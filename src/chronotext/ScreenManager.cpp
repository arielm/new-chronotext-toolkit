#include "chronotext/ScreenManager.h"

using namespace ci;
using namespace app;
using namespace std;

void ScreenManager::setup(bool renewContext)
{
    if (currentScreen && renewContext)
    {
        currentScreen->setup(true);
    }
}

void ScreenManager::shutdown()
{
    for (list<CinderSketch*>::const_iterator it = screens.begin(); it != screens.end(); ++it)
    {
        CinderSketch *screen = *it;

        screen->shutdown();
        delete screen;
    }
}

void ScreenManager::resize()
{
    if (currentScreen)
    {
        currentScreen->resize();
    }
}

void ScreenManager::update()
{
    if (currentScreen)
    {
        currentScreen->run(); // NECESSARY FOR THE "MESSAGE-PUMP"
        currentScreen->update();
    }
}

void ScreenManager::draw()
{
    if (currentScreen)
    {
        currentScreen->draw();
    }
}

void ScreenManager::start(int flags)
{
    if (currentScreen)
    {
        switch(flags)
        {
            case CinderSketch::FLAG_FOCUS_GAINED:
                if (!focused[currentScreen])
                {
                    focused[currentScreen] = true;
                    currentScreen->start(CinderSketch::FLAG_FOCUS_GAINED);
                }
                break;
                
            case CinderSketch::FLAG_APP_RESUME:
                currentScreen->start(CinderSketch::FLAG_APP_RESUME);
                break;
        }
    }
}

void ScreenManager::stop(int flags)
{
    if (currentScreen)
    {
        switch(flags)
        {
            case CinderSketch::FLAG_FOCUS_LOST:
                focused[currentScreen] = false;
                currentScreen->stop(CinderSketch::FLAG_FOCUS_LOST);
                break;
                
            case CinderSketch::FLAG_APP_PAUSE:
                currentScreen->stop(CinderSketch::FLAG_APP_PAUSE);
                break;
        }
    }
}

void ScreenManager::event(int id)
{
    if (currentScreen)
    {
        currentScreen->event(id);
    }
}

void ScreenManager::addTouch(int index, float x, float y)
{
    if (currentScreen)
    {
        currentScreen->addTouch(index, x, y);
    }
}

void ScreenManager::updateTouch(int index, float x, float y)
{
    if (currentScreen)
    {
        currentScreen->updateTouch(index, x, y);
    }
}

void ScreenManager::removeTouch(int index, float x, float y)
{
    if (currentScreen)
    {
        currentScreen->removeTouch(index, x, y);
    }
}

void ScreenManager::accelerated(AccelEvent event)
{
    if (currentScreen)
    {
        currentScreen->accelerated(event);
    }
}

void ScreenManager::handleMessage(const Message &message)
{
    if (currentScreen)
    {
        currentScreen->handleMessage(message);
    }
}

void ScreenManager::addScreen(CinderSketch *screen)
{
    screens.push_back(screen);
    focused[screen] = false;
}

void ScreenManager::removeScreen(CinderSketch *screen)
{
    screens.remove(screen);
    focused.erase(screen);

    screen->shutdown();
    delete screen;
}

void ScreenManager::setCurrentScreen(CinderSketch *screen)
{
    if (currentScreen)
    {
        currentScreen->stop(CinderSketch::FLAG_SCREEN_LEAVE);
    }
    
    if (screen)
    {
        /*
         * REQUIRED IN ORDER TO AVOID SITUATIONS WHERE A "SCREEN_ENTER"
         * IS FOLLOWED BY SOME UN-NECESSARY "FOCUS_GAIN"
         */
        focused[screen] = true;
        
        screen->resize();
        screen->start(CinderSketch::FLAG_SCREEN_ENTER);
    }
    
    currentScreen = screen;
}
