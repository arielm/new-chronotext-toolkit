/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/sound/Effect.h"
#include "chronotext/sound/SoundManager.h"
#include "chronotext/Context.h"

using namespace std;

namespace chr
{
    Effect::Effect(const Request &request, FMOD::Sound *sound, int uniqueId)
    :
    request(request),
    uniqueId(uniqueId),
    sound(nullptr)
    {
        setSound(sound);
    }
    
    Effect::~Effect()
    {
        resetSound();
    }

    int64_t Effect::getMemoryUsage() const
    {
        return memoryUsage;
    }

    double Effect::getDuration() const
    {
        return duration;
    }
    
    void Effect::setSound(FMOD::Sound *sound)
    {
        if (!sound)
        {
            resetSound();
        }
        else if (!Effect::sound)
        {
            Effect::sound = sound;
            
            memoryUsage = SoundManager::getSoundMemoryUsage(sound);
            duration = SoundManager::getSoundDuration(sound);
            
            // ---
            
            LOGI_IF(SoundManager::LOG_VERBOSE) <<
            "EFFECT LOADED: " <<
            request.inputSource->getFilePathHint() << " | " <<
            uniqueId << " | " <<
            utils::format::duration(duration) << " | " <<
            utils::format::bytes(memoryUsage) <<
            endl;
        }
    }
    
    void Effect::resetSound()
    {
        if (sound)
        {
            sound->release();
            sound = nullptr;
            
            // ---
            
            LOGI_IF(SoundManager::LOG_VERBOSE) <<
            "EFFECT DISCARDED: " <<
            uniqueId <<
            endl;
        }
    }
}
