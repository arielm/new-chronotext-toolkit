/*
 * FIXME: THE RESIZE BEHAVIOR SHOULD BE ON-PAR WITH chronotext-toolkit
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include <list>
#include <map>

class ScreenManager : public CinderSketch
{
    std::list<CinderSketch*> screens;
    std::map<CinderSketch*, int> focused;

public:
    CinderSketch *currentScreen;

    ScreenManager(void *context, void *delegate)
    :
    CinderSketch(context, delegate),
    currentScreen(NULL)
    {}

    void setup(bool renewContext);
    void shutdown();
    void resize();
    
    void update();
    void draw();
    
    void start(int flags);
    void stop(int flags);
    void event(int id);
    
    void addTouch(int index, float x, float y);
    void updateTouch(int index, float x, float y);
    void removeTouch(int index, float x, float y);

    void accelerated(AccelEvent event);

    void handleMessage(const Message &message);

    void addScreen(CinderSketch *screen);
    void removeScreen(CinderSketch *screen);
    
    void setCurrentScreen(CinderSketch *screen);
};
