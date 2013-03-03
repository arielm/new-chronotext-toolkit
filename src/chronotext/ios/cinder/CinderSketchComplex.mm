#include "chronotext/ios/cinder/CinderSketchComplex.h"

#import "CinderDelegate.h"

using namespace ci;
using namespace app;
using namespace std;

void CinderSketchComplex::touchesBegan(TouchEvent event)
{
	for (vector<TouchEvent::Touch>::const_iterator it = event.getTouches().begin(); it != event.getTouches().end(); ++it)
	{
		float x = it->getX();
		float y = it->getY();
		addTouch(it->getId() - 1, x, y);
	}
}

void CinderSketchComplex::touchesMoved(TouchEvent event)
{
	for (vector<TouchEvent::Touch>::const_iterator it = event.getTouches().begin(); it != event.getTouches().end(); ++it)
	{
		float x = it->getX();
		float y = it->getY();
		updateTouch(it->getId() - 1, x, y);
	}
}

void CinderSketchComplex::touchesEnded(TouchEvent event)
{
	for (vector<TouchEvent::Touch>::const_iterator it = event.getTouches().begin(); it != event.getTouches().end(); ++it)
	{
		float x = it->getX();
		float y = it->getY();
		removeTouch(it->getId() - 1, x, y);
	}
}

void CinderSketchComplex::enableAccelerometer(float updateFrequency, float filterFactor)
{
	[(CinderDelegate*)context setAccelFilterFactor:filterFactor];
	
	if (updateFrequency <= 0)
	{
		updateFrequency = 30;
	}
	
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1 / updateFrequency)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:(CinderDelegate*)context];
}

void CinderSketchComplex::disableAccelerometer()
{
	[[UIAccelerometer sharedAccelerometer] setDelegate:nil];
}

double CinderSketchComplex::getElapsedSeconds() const
{
    return ((CinderDelegate*)context).elapsedSeconds;
}

uint32_t CinderSketchComplex::getElapsedFrames() const
{
    return ((CinderDelegate*)context).elapsedFrames;
}

int CinderSketchComplex::getWindowWidth() const
{
    return ((CinderDelegate*)context).width;
}

int CinderSketchComplex::getWindowHeight() const
{
    return ((CinderDelegate*)context).height;
}

Vec2i CinderSketchComplex::getWindowSize() const
{
    return Vec2i(((CinderDelegate*)context).width, ((CinderDelegate*)context).height);
}

float CinderSketchComplex::getWindowAspectRatio() const
{
    return ((CinderDelegate*)context).width / (float)((CinderDelegate*)context).height;
}

Area CinderSketchComplex::getWindowBounds() const
{
    return Area(0, 0, ((CinderDelegate*)context).width, ((CinderDelegate*)context).height);
}

float CinderSketchComplex::getWindowContentScale() const
{
    return ((CinderDelegate*)context).contentScale;
}

void CinderSketchComplex::sendMessageToDelegate(int what, const string &body)
{
    if (delegate)
    {
        if (body.size() > 0)
        {
            [(CinderDelegate*)context receiveMessageFromSketch:what body:[NSString stringWithUTF8String:body.c_str()]];
        }
        else
        {
            [(CinderDelegate*)context receiveMessageFromSketch:what body:nil];
        }
    }
}
