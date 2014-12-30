/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SoundManager.h"

#include "chronotext/Context.h"

#include "fmod_errors.h"

using namespace std;
using namespace ci;

namespace chr
{
    SoundManager::SoundManager()
    :
    system(nullptr),
    playCount(0),
    effectCount(0)
    {}
    
    SoundManager::~SoundManager()
    {
        shutdown();
    }
    
    void SoundManager::setup(int maxChannels)
    {
        if (!system)
        {
            FMOD::System_Create(&system);
            system->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
            system->getMasterChannelGroup(&masterGroup);
        }
    }
    
    void SoundManager::shutdown()
    {
        if (system)
        {
            discardEffects();
            
            system->close();
            system->release();
            system = nullptr;
        }
    }
    
    void SoundManager::pause()
    {
        if (system)
        {
            masterGroup->setPaused(true);
        }
    }
    
    void SoundManager::resume()
    {
        if (system)
        {
            masterGroup->setPaused(false);
        }
    }
    
    void SoundManager::update()
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
                    completedEvents.emplace_back(Event(EVENT_COMPLETED, findByUniqueId(uniqueId), channelId, playingId));
                }
            }
            
            for (auto &event : completedEvents)
            {
                playingEffects.erase(event.playingId);
                dispatchEvent(event);
            }
        }
    }
    
    void SoundManager::addListener(Listener *listener)
    {
        listeners.insert(listener);
    }
    
    void SoundManager::removeListener(Listener *listener)
    {
        listeners.erase(listener);
    }
    
    Effect::Ref SoundManager::getEffect(const Effect::Request &request)
    {
        auto it = effects.find(request);
        
        if (it != effects.end())
        {
            return it->second;
        }
        
        auto sound = createSound(request); // CALLED BEFORE effectCount IS INCREMENTED, BECAUSE IT CAN THROW
        auto effect = Effect::Ref(new Effect(request, ++effectCount, sound));
        effects[request] = effect;
        
        return effect;
    }

    Effect::Ref SoundManager::findEffect(const Effect::Request &request) const
    {
        auto it = effects.find(request);
        
        if (it != effects.end())
        {
            return it->second;
        }
        
        return nullptr;
    }
    
    void SoundManager::discardEffect(Effect::Ref effect)
    {
        if (effect)
        {
            stopEffect(effect);
            effect->resetSound();
        }
    }
    
    bool SoundManager::reloadEffect(Effect::Ref effect)
    {
        if (effect)
        {
            if (!effect->sound)
            {
                effect->setSound(createSound(effect->request));
            }
            
            return effect->sound;
        }
        
        return false;
    }
    
    void SoundManager::discardEffects(int tag)
    {
        for (auto &element : effects)
        {
            if ((tag < 0) || (tag == element.second->request.tag))
            {
                discardEffect(element.second);
            }
        }
    }
    
    void SoundManager::reloadEffects(int tag)
    {
        for (auto &element : effects)
        {
            if ((tag < 0) || (tag == element.second->request.tag))
            {
                reloadEffect(element.second);
            }
        }
    }
    
    void SoundManager::stopEffect(Effect::Ref effect)
    {
        if (effect)
        {
            vector<int> playingIdsToStop;

            for (auto &element : playingEffects)
            {
                if (element.second.second == effect->uniqueId)
                {
                    playingIdsToStop.push_back(element.first);
                }
            }
            
            for (auto playingId : playingIdsToStop)
            {
                stopEffect(playingId);
            }
        }
    }
    
    void SoundManager::stopEffects(int tag)
    {
        vector<int> playingIdsToStop;
        
        for (auto &element : playingEffects)
        {
            playingIdsToStop.push_back(element.first);
        }
        
        for (auto &playingId : playingIdsToStop)
        {
            stopEffect(playingId, tag);
        }
    }
    
    int SoundManager::playEffect(Effect::Ref effect, int loopCount, float volume)
    {
        if (reloadEffect(effect))
        {
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
            
            if (volume == 1)
            {
                channel->setVolume(effect->request.volume);
            }
            else
            {
                channel->setVolume(volume);
            }
            
            channel->setPaused(false);
            
            // ---
            
            int channelId;
            channel->getIndex(&channelId);
            
            interruptChannel(channelId);
            
            int playingId = ++playCount;
            playingEffects[playingId] = make_pair(channelId, effect->uniqueId);
            
            dispatchEvent(Event(EVENT_STARTED, effect, channelId, playingId));
            return playingId;
        }
        
        return 0;
    }
    
    bool SoundManager::pauseEffect(int playingId, int tag)
    {
        auto it = playingEffects.find(playingId);
        
        if (it != playingEffects.end())
        {
            auto channelId = it->second.first;
            auto effect = findByUniqueId(it->second.second);
            
            if ((tag < 0) || (effect->request.tag == tag))
            {
                FMOD::Channel *channel;
                system->getChannel(channelId, &channel);
                channel->setPaused(true);
                
                return true;
            }
        }
        
        return false;
    }
    
    bool SoundManager::resumeEffect(int playingId, int tag)
    {
        auto it = playingEffects.find(playingId);
        
        if (it != playingEffects.end())
        {
            auto channelId = it->second.first;
            auto effect = findByUniqueId(it->second.second);
            
            if ((tag < 0) || (effect->request.tag == tag))
            {
                FMOD::Channel *channel;
                system->getChannel(channelId, &channel);
                channel->setPaused(false);
                
                return true;
            }
        }
        
        return false;
    }
    
    bool SoundManager::stopEffect(int playingId, int tag)
    {
        auto it = playingEffects.find(playingId);
        
        if (it != playingEffects.end())
        {
            auto channelId = it->second.first;
            auto effect = findByUniqueId(it->second.second);
            
            if ((tag < 0) || (effect->request.tag == tag))
            {
                FMOD::Channel *channel;
                system->getChannel(channelId, &channel);
                channel->stop();
                
                playingEffects.erase(it);
                dispatchEvent(Event(EVENT_STOPPED, effect, channelId, playingId));
                
                return true;
            }
        }
        
        return false;
    }
    
    void SoundManager::setMute(bool mute)
    {
        if (system)
        {
            masterGroup->setMute(mute);
        }
    }
    
    bool SoundManager::isMute()
    {
        bool mute = false;
        
        if (system)
        {
            masterGroup->getMute(&mute);
        }
        
        return mute;
    }
    
    float SoundManager::getVolume()
    {
        float volume = 0;
        
        if (system)
        {
            masterGroup->getVolume(&volume);
        }
        
        return volume;
    }
    
    void SoundManager::setVolume(float volume)
    {
        if (system)
        {
            masterGroup->setVolume(volume);
        }
    }
    
    bool SoundManager::interruptChannel(int channelId)
    {
        for (auto &element : playingEffects)
        {
            if (channelId == element.second.first)
            {
                auto playingId = element.first;
                auto effect = findByUniqueId(element.second.second);
                
                playingEffects.erase(playingId);
                dispatchEvent(Event(EVENT_INTERRUPTED, effect, channelId, playingId));
                
                return true;
            }
        }
        
        return false;
    }
    
    Effect::Ref SoundManager::findByUniqueId(int uniqueId)
    {
        for (auto &element : effects)
        {
            if (uniqueId == element.second->uniqueId)
            {
                return element.second;
            }
        }
        
        assert(false); // UNREACHABLE (AS LONG AS THE FUNCTION IS NOT PUBLIC)
    }
    
    void SoundManager::dispatchEvent(const Event &event)
    {
        for (auto &listener : listeners)
        {
            listener->handleEvent(event);
        }
    }
    
    FMOD::Sound* SoundManager::createSound(const Effect::Request &request)
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
            throw EXCEPTION(SoundManager, FMOD_ErrorString(result));
        }
        
        return sound;
    }
}
