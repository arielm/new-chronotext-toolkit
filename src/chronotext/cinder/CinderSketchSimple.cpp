/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/cinder/CinderSketchSimple.h"
#include "chronotext/cinder/CinderApp.h"

using namespace std;
using namespace ci;
using namespace app;

CinderSketchSimple::CinderSketchSimple(void *context, void *delegate)
:
CinderSketchBase(),
context(static_cast<AppNative*>(context)),
delegate(static_cast<CinderApp*>(delegate))
{}

void CinderSketchSimple::sendMessageToDelegate(int what, const string &body)
{
    if (delegate)
    {
        delegate->receiveMessageFromSketch(what, body);
    }
}
