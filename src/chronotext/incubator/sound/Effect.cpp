/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Effect.h"

#include "chronotext/utils/Utils.h"

using namespace std;

Effect::Effect(InputSourceRef inputSource, FMOD::Sound *sound)
:
inputSource(inputSource),
sound(sound)
{
    unsigned int length;
    sound->getLength(&length, FMOD_TIMEUNIT_MS);
    
    unsigned int memoryused;
    sound->getMemoryInfo(FMOD_MEMBITS_SOUND, 0, &memoryused, NULL);
    
    duration = length / 1000.0;
    
    LOGD <<
    "EFFECT LOADED: " <<
    inputSource->getFilePathHint() << " | " <<
    length / 1000.0 << "s | " <<
    memoryused << " BYTES" <<
    endl;
}

Effect::~Effect()
{
    sound->release();
    
    // ---
    
    LOGD <<
    "EFFECT UNLOADED: " <<
    inputSource->getFilePathHint() <<
    endl;
}

double Effect::getDuration()
{
    return duration;
}