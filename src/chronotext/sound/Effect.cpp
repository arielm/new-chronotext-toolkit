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
    Effect::Effect(const Request &request, int uniqueId, FMOD::Sound *sound)
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
    
    double Effect::getDuration() const
    {
        if (sound)
        {
            unsigned int length;
            sound->getLength(&length, FMOD_TIMEUNIT_MS);
            
            return length / 1000.0;
        }
        
        return 0;
    }
    
    int64_t Effect::getMemoryUsage() const
    {
        if (sound)
        {
            /*
             * TODO: CONSIDER MEASURING ONCE, UPON CONSTRUCTION
             *
             * FIXME: MEASURE DOES NOT SEEM ACCURATE FOR CERTAIN SOUND (ACCORDING TO DATA PRINTED BY DEBUG-VERSION OF FMOD...)
             */

            unsigned int memoryused;
            sound->getMemoryInfo(FMOD_MEMBITS_SOUND, 0, &memoryused, nullptr);
            
            return memoryused;
        }
        
        return 0;
    }
    
    void Effect::setSound(FMOD::Sound *sound)
    {
        if (!Effect::sound)
        {
            Effect::sound = sound;
            
            // ---
            
            LOGI_IF(SoundManager::LOG_VERBOSE) <<
            "EFFECT CREATED: " <<
            request.inputSource->getFilePathHint() << " | " <<
            getDuration() << "s | " <<
            utils::format::bytes(getMemoryUsage()) <<
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
            "EFFECT DESTROYED: " <<
            request.inputSource->getFilePathHint() <<
            endl;
        }
    }
}
