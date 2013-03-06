#include "chronotext/android/cinder/CinderSketchComplex.h"
#include "chronotext/android/cinder/CinderDelegate.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void CinderSketchComplex::enableAccelerometer(float updateFrequency, float filterFactor)
{
	context->enableAccelerometer(updateFrequency, filterFactor);
}

void CinderSketchComplex::disableAccelerometer()
{
	context->disableAccelerometer();
}

double CinderSketchComplex::getElapsedSeconds() const
{
    return context->getElapsedSeconds();
}

uint32_t CinderSketchComplex::getElapsedFrames() const
{
    return context->getElapsedFrames();
}

int CinderSketchComplex::getWindowWidth()
{
    return context->getWindowWidth();
}

int CinderSketchComplex::getWindowHeight()
{
    return context->getWindowHeight();
}

Vec2f CinderSketchComplex::getWindowSize()
{
    return context->getWindowSize();
}

float CinderSketchComplex::getWindowAspectRatio()
{
    return context->getWindowAspectRatio();
}

Area CinderSketchComplex::getWindowBounds() const
{
    return context->getWindowBounds();
}

ostream& CinderSketchComplex::console()
{
    return context->console();
}

void CinderSketchComplex::sendMessageToDelegate(int what, const string &body)
{
    if (delegate)
    {
        delegate->receiveMessageFromSketch(what, body);
    }
}
