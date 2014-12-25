/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "FMOD.hpp"

namespace chr
{
    class Effect
    {
    public:
        static bool VERBOSE;
        
        typedef std::shared_ptr<Effect> Ref;
        
        struct Request
        {
            InputSource::Ref inputSource;
            float volume;
            bool forceMemoryLoad;
            
            Request(InputSource::Ref inputSource, float volume = 1, bool forceMemoryLoad = false)
            :
            inputSource(inputSource),
            volume(volume),
            forceMemoryLoad(forceMemoryLoad)
            {}
            
            bool operator<(const Request &rhs) const
            {
                return (inputSource->getURI() < rhs.inputSource->getURI());
            }
        };
        
        Request request;
        int uniqueId;
        FMOD::Sound *sound;
        
        Effect(const Request &request, int uniqueId, FMOD::Sound *sound);
        ~Effect();
        
        double getDuration();
        size_t getMemoryUsage();
    };
}