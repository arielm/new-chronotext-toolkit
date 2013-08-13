#pragma once

#include "chronotext/InputSource.h"

#include "FMOD.hpp"

#include <memory>

typedef std::shared_ptr<class Effect> EffectRef;

class Effect
{
public:
    InputSourceRef inputSource;
    FMOD::Sound *sound;
    
    Effect(InputSourceRef inputSource, FMOD::Sound *sound);
    ~Effect();
    
    double getDuration();

protected:
    double duration;
};
