/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SoundEngine.h"

#include "chronotext/Context.h"

#include "fmod_errors.h"

using namespace std;
using namespace ci;

namespace chr
{
    SoundEngine::SoundEngine()
    :
    system(nullptr),
    playCount(0),
    effectCount(0)
    {}
    
    void SoundEngine::setup(int maxChannels)
    {
        if (!system)
        {
            FMOD::System_Create(&system);
            system->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
            system->getMasterChannelGroup(&masterGroup);
        }
    }
    
    void SoundEngine::shutdown()
    {
        if (system)
        {
            system->close();
            system->release();
            system = nullptr;
        }
    }
    
    void SoundEngine::pause()
    {
        if (system)
        {
            masterGroup->setPaused(true);
        }
    }
    
    void SoundEngine::resume()
    {
        if (system)
        {
            masterGroup->setPaused(false);
        }
    }
    
    void SoundEngine::update()
    {
        if (system)
        {
            system->update();
            
            // ---
            
            vector<Event> completedEvents;
            
            for (auto &element : playingEffects)
            {
                auto playingId = element.first;
                auto channelId = element.second.first;
                auto uniqueId = element.second.second;
                
                FMOD::Channel *channel;
                system->getChannel(channelId, &channel);
                
                bool playing;
                channel->isPlaying(&playing);
                
                if (!playing)
                {
                    completedEvents.emplace_back(createEvent(EVENT_COMPLETED, uniqueId, channelId, playingId));
                }
            }
            
            for (auto &event : completedEvents)
            {
                playingEffects.erase(event.playingId);
                dispatchEvent(event);
            }
        }
    }
    
    void SoundEngine::addListener(Listener *listener)
    {
        listeners.insert(listener);
    }
    
    void SoundEngine::removeListener(Listener *listener)
    {
        listeners.erase(listener);
    }
    
    Effect::Ref SoundEngine::preloadEffect(const Effect::Request &request)
    {
        auto element = effects.find(request);
        
        if (element == effects.end())
        {
            auto effect = Effect::Ref(loadEffect(request)); // CAN THROW
            effects[request] = effect;
            return effect;
        }
        else
        {
            return element->second;
        }
    }
    
    bool SoundEngine::unloadEffect(const Effect::Request &request)
    {
        auto element = effects.find(request);
        
        if (element != effects.end())
        {
            stopEffects(element->second->uniqueId);
            effects.erase(element); // TODO INSTEAD: "DISCARD" EFFECT
            
            return true;
        }
        
        return false;
    }
    
    Effect::Ref SoundEngine::getEffect(const Effect::Request &request)
    {
        auto element = effects.find(request);
        
        if (element != effects.end())
        {
            return element->second;
        }
        
        return nullptr;
    }
    
    int SoundEngine::playEffect(int uniqueId, int loopCount, float volume)
    {
        for (auto &element : effects)
        {
            if (element.second->uniqueId == uniqueId)
            {
                auto effect = element.second;
                
                FMOD::Channel *channel;
                system->playSound(FMOD_CHANNEL_FREE, effect->sound, true, &channel);
                
                if (loopCount)
                {
                    channel->setLoopCount(loopCount);
                    channel->setMode(FMOD_LOOP_NORMAL);
                }
                else
                {
                    channel->setMode(FMOD_LOOP_OFF);
                }
                
                channel->setVolume(volume);
                channel->setPaused(false);
                
                // ---
                
                int channelId;
                channel->getIndex(&channelId);
                
                interruptChannel(channelId);
                
                int playingId = ++playCount;
                playingEffects[playingId] = make_pair(channelId, uniqueId);
                
                dispatchEvent(Event(EVENT_STARTED, effect, channelId, playingId));
                return playingId;
            }
        }
        
        return 0;
    }
    
    bool SoundEngine::pauseEffect(int playingId)
    {
        auto element = playingEffects.find(playingId);
        
        if (element != playingEffects.end())
        {
            FMOD::Channel *channel;
            system->getChannel(element->second.first, &channel);
            channel->setPaused(true);
            
            return true;
        }
        
        return false;
    }
    
    bool SoundEngine::resumeEffect(int playingId)
    {
        auto element = playingEffects.find(playingId);
        
        if (element != playingEffects.end())
        {
            FMOD::Channel *channel;
            system->getChannel(element->second.first, &channel);
            channel->setPaused(false);
            
            return true;
        }
        
        return false;
    }
    
    bool SoundEngine::stopEffect(int playingId)
    {
        auto element = playingEffects.find(playingId);
        
        if (element != playingEffects.end())
        {
            auto channelId = element->second.first;
            auto uniqueId = element->second.second;
            
            FMOD::Channel *channel;
            system->getChannel(channelId, &channel);
            channel->stop();
            
            playingEffects.erase(element);
            dispatchEvent(createEvent(EVENT_STOPPED, uniqueId, channelId, playingId));
            
            return true;
        }
        
        return false;
    }
    
    bool SoundEngine::stopEffects(int uniqueId)
    {
        vector<int> playingIdsToStop;
        
        for (auto &element : playingEffects)
        {
            if (element.second.second == uniqueId)
            {
                playingIdsToStop.push_back(element.first);
            }
        }
        
        for (auto playingId : playingIdsToStop)
        {
            stopEffect(playingId);
        }
        
        return (!playingIdsToStop.empty());
    }
    
    bool SoundEngine::stopAllEffects()
    {
        vector<int> playingIdsToStop;
        
        for (auto &element : playingEffects)
        {
            playingIdsToStop.push_back(element.first);
        }
        
        for (auto &playingId : playingIdsToStop)
        {
            stopEffect(playingId);
        }
        
        return (!playingIdsToStop.empty());
    }
    
    void SoundEngine::setMute(bool mute)
    {
        if (system)
        {
            masterGroup->setMute(mute);
        }
    }
    
    bool SoundEngine::isMute()
    {
        bool mute = false;
        
        if (system)
        {
            masterGroup->getMute(&mute);
        }
        
        return mute;
    }
    
    float SoundEngine::getVolume()
    {
        float volume = 0;
        
        if (system)
        {
            masterGroup->getVolume(&volume);
        }
        
        return volume;
    }
    
    void SoundEngine::setVolume(float volume)
    {
        if (system)
        {
            masterGroup->setVolume(volume);
        }
    }
    
    Effect* SoundEngine::loadEffect(const Effect::Request &request)
    {
        FMOD_RESULT result = FMOD_ERR_UNINITIALIZED;
        FMOD::Sound *sound = nullptr;
        
        if (system)
        {
            if (request.forceMemoryLoad || !request.inputSource->isFile())
            {
                auto buffer = request.inputSource->loadDataSource()->getBuffer();
                
                FMOD_CREATESOUNDEXINFO exinfo;
                memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
                exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
                exinfo.length = buffer.getDataSize();
                
                result = system->createSound(static_cast<const char*>(buffer.getData()), FMOD_DEFAULT | FMOD_OPENMEMORY, &exinfo, &sound);
            }
            else
            {
                result = system->createSound(request.inputSource->getFilePath().c_str(), FMOD_DEFAULT, nullptr, &sound);
            }
        }
        
        if (result)
        {
            throw EXCEPTION(SoundEngine, FMOD_ErrorString(result));
        }
        
        return new Effect(request, ++effectCount, sound);
    }
    
    bool SoundEngine::interruptChannel(int channelId)
    {
        for (auto &element : playingEffects)
        {
            if (element.second.first == channelId)
            {
                auto playingId = element.first;
                auto uniqueId = element.second.second;
                
                playingEffects.erase(playingId);
                dispatchEvent(createEvent(EVENT_INTERRUPTED, uniqueId, channelId, playingId));
                
                return true;
            }
        }
        
        return false;
    }
    
    SoundEngine::Event SoundEngine::createEvent(Type type, int uniqueId, int channelId, int playingId)
    {
        for (auto &element : effects)
        {
            if (element.second->uniqueId == uniqueId)
            {
                return Event(type, element.second, channelId, playingId);
            }
        }
        
        return Event();
    }
    
    void SoundEngine::dispatchEvent(const Event &event)
    {
        if (event.type != EVENT_UNDEFINED)
        {
            for (auto &listener : listeners)
            {
                listener->handleEvent(event);
            }
        }
    }
}
