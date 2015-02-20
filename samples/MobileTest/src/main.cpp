/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/Context.h"

#include "Sketch.h"

namespace chr
{
    CinderSketch* createSketch()
    {
        return new Sketch();
    }
}

#pragma mark ----------------------------------------   IOS   ----------------------------------------

#if defined(CINDER_COCOA_TOUCH)

#include "Application.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([Application class]));
    }
}

#pragma mark ---------------------------------------- ANDROID ----------------------------------------

#elif defined(CINDER_ANDROID)

extern "C"
{}

#pragma mark ----------------------------------------   OSX   ----------------------------------------

#elif defined(CINDER_MAC)

#include "Application.h"

@class CinderBridge;

namespace chr
{
    CinderBridge* createBridge(CinderDelegate *delegate)
    {
        return nil;
    }
}

CINDER_APP_NATIVE(Application, ci::app::RendererGl(ci::app::RendererGl::AA_NONE))

#endif
