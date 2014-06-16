/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "FMOD.hpp"

#include <memory>

typedef std::shared_ptr<class Effect> EffectRef;

class Effect
{
public:
    chr::InputSourceRef inputSource;
    FMOD::Sound *sound;
    
    Effect(chr::InputSourceRef inputSource, FMOD::Sound *sound);
    ~Effect();
    
    double getDuration();

protected:
    double duration;
};
