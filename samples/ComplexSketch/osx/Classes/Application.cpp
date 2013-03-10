/*
 * TODO:
 * MSW AND ANDROID VERSIONS
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
    settings->setWindowSize(320, 480);
#endif
}

CINDER_APP_NATIVE(Application, RendererGl(0))
