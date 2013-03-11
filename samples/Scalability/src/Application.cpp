/*
 * STUB FOR RUNNING A CinderSketch
 * ON THE DESKTOP (OSX OR WINDOW)
 */

#include "chronotext/cinder/CinderApp.h"

#include "Sketch.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Application : public CinderApp
{
public:
    Application();
    void prepareSettings(Settings *settings);
};

Application::Application()
{
    sketch = new Sketch(this);
}

void Application::prepareSettings(Settings *settings)
{
#if defined(CINDER_MAC) || defined(CINDER_MSW)
//  settings->setWindowSize(320, 480); // IPHONE
    settings->setWindowSize(640, 960); // IPHONE RETINA
//  settings->setWindowSize(768, 1024); // IPAD-1
//  settings->setWindowSize(480, 800); // ANDROID WVGA800
#endif
}

CINDER_APP_NATIVE(Application, RendererGl(0))
