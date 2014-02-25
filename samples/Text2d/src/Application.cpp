/*
 * STUB FOR RUNNING A CinderSketch
 * ON THE DESKTOP (OSX OR WINDOW)
 */

#include "chronotext/cinder/CinderApp.h"

#include "Sketch.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

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
    settings->enableHighDensityDisplay();

//  settings->setWindowSize(480, 320); // IPHONE
//  settings->setWindowSize(960, 640); // IPHONE RETINA
    settings->setWindowSize(1024, 768); // IPAD-1
//  settings->setWindowSize(800, 480); // ANDROID WVGA800
}

CINDER_APP_NATIVE(Application, RendererGl(RendererGl::AA_NONE))
