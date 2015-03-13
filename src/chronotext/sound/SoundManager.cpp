/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/sound/SoundManager.h"
#include "chronotext/Context.h"

#include "fmod_errors.h"

using namespace std;
using namespace ci;

namespace chr
{
    atomic<bool> SoundManager::LOG_VERBOSE (false);
    atomic<bool> SoundManager::PROBE_MEMORY (false);
    
    SoundManager::~SoundManager()
    {
        uninit();
    }
    
    bool SoundManager::isValid()
    {
        return initialized;
    }
    
    bool SoundManager::init(int maxChannels, FMOD_INITFLAGS flags)
    {
        if (!initialized)
        {
            FMOD::System_Create(&system);
            FMOD_RESULT result = system->init(maxChannels, flags, nullptr);
            
            if (result)
            {
                system->release();
                system = nullptr;
            }
            else
            {
                system->getMasterChannelGroup(&masterGroup);
                initialized = true;
            }
        }
        
        return initialized;
    }
    
    void SoundManager::uninit()
    {
        if (initialized)
        {
            discardEffects();
            assert(playingEffects.empty());

            effects.clear();
            listeners.clear();
            
            system->close();
            system->release();
            system = nullptr;
            
            // ---
            
            initialized = false;
        }
    }
    
    void SoundManager::pause()
    {
        if (initialized)
        {
            masterGroup->setPaused(true);
            system->update(); // NECESSARY ON ANDROID, OTHERWISE PAUSE-REQUEST WILL NOT BE EFFECTIVE
        }
    }
    
    void SoundManager::resume()
    {
        if (initialized)
        {
            masterGroup->setPaused(false);
            system->update(); // PROBABLY NOT PURELY NECESSARY...
        }
    }
    
    void SoundManager::update()
    {
        if (initialized)
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
        if (initialized)
        {
            auto it = effects.find(request);
            
            if (it != effects.end())
            {
                return it->second;
            }
            
            auto sound = loadSound(system, request); // CAN THROW
            auto effect = Effect::Ref(new Effect(request, sound, ++effectCounter)); // make_shared CAN'T BE USED WITH PROTECTED CONSTRUCTORS
            effects[request] = effect;
            
            return effect;
        }
        
        return nullptr;
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
        if (initialized && effect)
        {
            stopEffect(effect);
            effect->resetSound();
        }
    }
    
    bool SoundManager::reloadEffect(Effect::Ref effect)
    {
        if (initialized && effect)
        {
            if (!effect->sound)
            {
                effect->setSound(loadSound(system, effect->request));
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
        if (initialized)
        {
            masterGroup->setMute(mute);
        }
    }
    
    bool SoundManager::isMute()
    {
        bool mute = false;
        
        if (initialized)
        {
            masterGroup->getMute(&mute);
        }
        
        return mute;
    }
    
    float SoundManager::getVolume()
    {
        float volume = 0;
        
        if (initialized)
        {
            masterGroup->getVolume(&volume);
        }
        
        return volume;
    }
    
    void SoundManager::setVolume(float volume)
    {
        if (initialized)
        {
            masterGroup->setVolume(volume);
        }
    }
    
    int SoundManager::playEffect(Effect::Ref effect, int loopCount, float volume)
    {
        if (reloadEffect(effect))
        {
            auto channel = playSound(effect->sound, loopCount, (volume == 1) ? effect->request.volume : volume);
            
            int channelId;
            channel->getIndex(&channelId);
            
            interruptChannel(channelId);
            
            int playingId = ++playCounter;
            playingEffects[playingId] = make_pair(channelId, effect->uniqueId);
            
            dispatchEvent(Event(EVENT_STARTED, effect, channelId, playingId));
            return playingId;
        }
        
        return 0;
    }
    
    FMOD::Channel* SoundManager::playSound(FMOD::Sound *sound, int loopCount, float volume)
    {
        if (initialized)
        {
            FMOD::Channel *channel;
            FMOD_RESULT result = system->playSound(sound, masterGroup, true, &channel);
            
            if (result)
            {
                throw EXCEPTION(SoundManager, "UNABLE TO PLAY SOUND | REASON: " + writeError(result));
            }
            
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
            
            return channel;
        }
        
        return nullptr;
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
        
        assert(false); // UNREACHABLE (AS LONG AS THE FUNCTION IS NOT PUBLIC: uniqueId IS SAFE)
    }
    
    void SoundManager::dispatchEvent(const Event &event)
    {
        for (auto &listener : listeners)
        {
            listener->handleEvent(event);
        }
    }
    
    // ---
    
    MemoryInfo SoundManager::memoryInfo[1];
    map<FMOD::Sound*, SoundManager::Record> SoundManager::records;
    
    FMOD::Sound* SoundManager::loadSound(FMOD::System *system, const Effect::Request &request)
    {
        FMOD::Sound *sound = nullptr;

        int currentalloced1;
        int maxalloced1;
        FMOD_RESULT result = FMOD::Memory_GetStats(&currentalloced1, &maxalloced1);
        
        if (!result)
        {
            if (SoundManager::PROBE_MEMORY)
            {
                memoryInfo[0] = getMemoryInfo();
            }

            if (request.forceMemoryLoad || !request.inputSource->hasFileName())
            {
                auto buffer = request.inputSource->loadDataSource()->getBuffer();
                
                FMOD_CREATESOUNDEXINFO exinfo;
                memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
                exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
                exinfo.length = (unsigned int)buffer.getDataSize();
                
                result = system->createSound(static_cast<const char*>(buffer.getData()), FMOD_DEFAULT | FMOD_OPENMEMORY, &exinfo, &sound);
            }
            else
            {
                result = system->createSound(request.inputSource->getFilePath().c_str(), FMOD_DEFAULT, nullptr, &sound);
            }
        }
        
        if (result)
        {
            throw EXCEPTION(SoundManager, "UNABLE TO LOAD SOUND | REASON: " + writeError(result));
        }

        if (sound)
        {
            int currentalloced2;
            int maxalloced2;
            FMOD::Memory_GetStats(&currentalloced2, &maxalloced2);
            
            auto memoryUsage = currentalloced2 - currentalloced1;
            records[sound] = Record({memoryUsage, memoryInfo[0]});
        }
        
        return sound;
    }
    
    double SoundManager::getSoundDuration(FMOD::Sound *sound)
    {
        if (sound)
        {
            unsigned int length;
            sound->getLength(&length, FMOD_TIMEUNIT_MS);
            
            return length / 1000.0;
        }
        
        return 0;
    }
    
    string SoundManager::writeError(FMOD_RESULT result)
    {
        return string(FMOD_ErrorString(result));
    }
}
