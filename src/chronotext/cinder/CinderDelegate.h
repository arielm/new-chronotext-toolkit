/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Cinder.h"

#if defined(CINDER_COCOA_TOUCH)

#include "chronotext/ios/cinder/CinderDelegate.h"

#elif defined(CINDER_ANDROID)

#include "chronotext/android/cinder/CinderDelegate.h"

#elif defined(CINDER_MAC)

#include "chronotext/osx/cinder/CinderDelegate.h"

#define CINDER_APP_DESKTOP(APP, AA) \
    @class CinderBridge; \
    namespace chr { CinderBridge* createBridge(CinderDelegate *delegate) { return nil; } } \
    CINDER_APP_NATIVE(APP, ci::app::RendererGl(AA))

#define CINDER_APP_OSX(APP, BRIDGE, AA) \
    @class CinderBridge; \
    namespace chr { CinderBridge* createBridge(CinderDelegate *delegate) { return [[BRIDGE alloc] initWithDelegate:delegate]; } } \
    CINDER_APP_NATIVE(APP, ci::app::RendererGl(AA))

#endif
