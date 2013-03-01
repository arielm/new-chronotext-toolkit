#include "chronotext/cinder/CinderSketchSimple.h"
#include "chronotext/cinder/CinderApp.h"

using namespace std;
using namespace ci;
using namespace app;

void CinderSketchSimple::sendMessageToDelegate(int what, const string &body)
{
    if (delegate)
    {
        delegate->receiveMessageFromSketch(what, body);
    }
}
