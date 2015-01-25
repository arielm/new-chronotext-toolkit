/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "fmod.hpp"

namespace chr
{
    class SoundManager;
    
    class Effect
    {
    public:
        typedef std::shared_ptr<Effect> Ref;
        
        struct Request
        {
            int tag;
            InputSource::Ref inputSource;
            
            float volume;
            bool forceMemoryLoad;
            
            Request(InputSource::Ref inputSource, float volume = 1, bool forceMemoryLoad = false)
            :
            tag(0),
            inputSource(inputSource),
            volume(volume),
            forceMemoryLoad(forceMemoryLoad)
            {}
            
            Request& setTag(int tag)
            {
                Request::tag = tag;
                return *this;
            }
            
            bool operator<(const Request &rhs) const
            {
                return (inputSource->getURI() < rhs.inputSource->getURI());
            }
        };
        
        // ---
        
        Request request;
        int uniqueId;
        FMOD::Sound *sound;
        
        ~Effect();

        double getDuration() const;
        int64_t getMemoryUsage() const;
        
    protected:
        friend SoundManager;

        int64_t memoryUsage;
        double duration;
        
        Effect(const Effect &other) = delete;
        Effect(const Request &request, FMOD::Sound *sound, int uniqueId = -1);

        void setSound(FMOD::Sound *sound);
        void resetSound();
    };
}
