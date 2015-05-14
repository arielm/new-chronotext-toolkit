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
using namespace chr::utils;

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
            auto it = SoundManager::records.find(sound);
            
            if (it == SoundManager::records.end())
            {
                throw EXCEPTION(Effect, "UNREGISTERED SOUND");
            }
            
            const auto &record = it->second;

            // ---
            
            Effect::sound = sound;
            memoryUsage = record.memoryUsage;
            
            duration = SoundManager::getSoundDuration(sound);

            // ---
            
            stringstream memoryStats;
            
            if (SoundManager::PROBE_MEMORY)
            {
                auto memoryInfo = getMemoryInfo();
                auto delta = memory::compare(record.memoryInfo[0], memoryInfo);
                
                memoryStats << " | " <<
                format::bytes(memoryUsage) << ", " <<
                format::bytes(delta) << " " <<
                memoryInfo;
            }
            
            LOGI_IF(SoundManager::LOG_VERBOSE) <<
            "EFFECT LOADED: " <<
            request.inputSource->getFilePathHint() << " | " <<
            uniqueId << " | " <<
            utils::format::duration(duration) <<
            memoryStats.str() <<
            endl;
        }
    }
    
    void Effect::resetSound()
    {
        if (sound)
        {
            MemoryInfo memoryInfo1;
            
            if (SoundManager::PROBE_MEMORY)
            {
                memoryInfo1 = getMemoryInfo();
            }
            
            sound->release();
            sound = nullptr;
            
            // ---
            
            stringstream memoryStats;
            
            if (SoundManager::PROBE_MEMORY)
            {
                auto memoryInfo2 = getMemoryInfo();
                auto delta = memory::compare(memoryInfo1, memoryInfo2);
                
                memoryStats << " | " <<
                format::bytes(memoryUsage) << ", " <<
                format::bytes(delta) << " " <<
                memoryInfo2;
            }
            
            LOGI_IF(SoundManager::LOG_VERBOSE) <<
            "EFFECT DISCARDED: " <<
            uniqueId <<
            memoryStats.str() <<
            endl;
        }
    }
}
