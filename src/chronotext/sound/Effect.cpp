/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Effect.h"

#include "chronotext/Context.h"
#include "chronotext/utils/Utils.h"

using namespace std;

namespace chr
{
    Effect::Effect(InputSource::Ref inputSource, uint32_t effectId, FMOD::Sound *sound)
    :
    inputSource(inputSource),
    effectId(effectId),
    sound(sound)
    {
        LOGD <<
        "EFFECT LOADED: " <<
        inputSource->getFilePathHint() << " | " <<
        getDuration() << "s | " <<
        prettyBytes(getMemoryUsage()) <<
        endl;
    }
    
    Effect::~Effect()
    {
        sound->release();
        sound = nullptr;
        
        // ---
        
        LOGD <<
        "EFFECT UNLOADED: " <<
        inputSource->getFilePathHint() <<
        endl;
    }
    
    uint32_t Effect::getId() const
    {
        return effectId;
    }
    
    double Effect::getDuration()
    {
        if (sound)
        {
            unsigned int length;
            sound->getLength(&length, FMOD_TIMEUNIT_MS);
            
            return length / 1000.0;
        }
        
        return 0;
    }
    
    size_t Effect::getMemoryUsage()
    {
        if (sound)
        {
            unsigned int memoryused;
            sound->getMemoryInfo(FMOD_MEMBITS_SOUND, 0, &memoryused, nullptr);
            
            return memoryused;
        }
        
        return 0;
    }
}
