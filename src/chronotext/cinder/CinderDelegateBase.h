/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

class CinderDelegateBase
{
public:
    virtual ~CinderDelegateBase() {}
    
    virtual void action(int actionId) {}
    virtual void receiveMessageFromSketch(int what, const std::string &body) {}
    virtual void sendMessageToSketch(int what, const std::string &body = "") {}

    virtual void sketchCreated(chr::CinderSketch *sketch) {}
    virtual void sketchDestroyed(chr::CinderSketch *sketch) {}

    virtual chr::CinderSketch* getSketch() = 0;
    virtual void setSketch(chr::CinderSketch *sketch) = 0;
};
