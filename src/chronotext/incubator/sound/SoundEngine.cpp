/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "SoundEngine.h"
#include "FMODException.h"

using namespace std;
using namespace ci;
using namespace chr;

SoundEngine::SoundEngine()
:
playCount(0),
listener(nullptr)
{}

void SoundEngine::setup(int maxChannels)
{
    FMOD::System_Create(&system);
    system->init(maxChannels, FMOD_INIT_NORMAL, NULL);
    system->getMasterChannelGroup(&masterGroup);
}

void SoundEngine::shutdown()
{
    system->close();
    system->release();
}

void SoundEngine::setListener(Listener *listener)
{
    this->listener = listener;
}

void SoundEngine::pause()
{
    masterGroup->setPaused(true);
}

void SoundEngine::resume()
{
    masterGroup->setPaused(false);
}

void SoundEngine::setMute(bool mute)
{
    masterGroup->setMute(mute);
}

void SoundEngine::setVolume(float volume)
{
    masterGroup->setVolume(volume);
}

void SoundEngine::update()
{
    system->update();
    
    // ---
    
    vector<Event> completedEvents;
    
    for (auto &it : playingEffects)
    {
        int playingId = it.first;
        int channelId = it.second.first;
        EffectRef effect = it.second.second;
        
        FMOD::Channel *channel;
        system->getChannel(channelId, &channel);
        
        bool playing;
        channel->isPlaying(&playing);
        
        if (!playing)
        {
            completedEvents.push_back(Event(EVENT_COMPLETED, effect, channelId, playingId));
        }
    }
    
    for (auto event : completedEvents)
    {
        playingEffects.erase(event.playingId);
        processEvent(event);
    }
}

EffectRef SoundEngine::preloadEffect(InputSourceRef inputSource)
{
    string key = inputSource->getURI();
    auto it = effects.find(key);
    
    if (it == effects.end())
    {
        EffectRef effect = loadEffect(inputSource);
        effects[key] = effect;
        return effect;
    }
    else
    {
        return it->second;
    }
}

void SoundEngine::unloadEffect(EffectRef effect)
{
    stopEffects(effect);
    
    // ---
    
    auto it = effects.find(effect->inputSource->getURI());
    
    if (it != effects.end())
    {
        effects.erase(it);
    }
}

int SoundEngine::playEffect(EffectRef effect, int loopCount, float volume)
{
    if (effect)
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

        channel->setVolume(volume);
        channel->setPaused(false);
        
        // ---
        
        int channelId;
        channel->getIndex(&channelId);
        
        interruptChannel(channelId);
        
        int playingId = nextPlayingId(effect);
        playingEffects[playingId] = make_pair(channelId, effect);
        
        processEvent(Event(EVENT_STARTED, effect, channelId, playingId));
        return playingId;
    }
    else
    {
        return 0;
    }
}

bool SoundEngine::stopEffect(int playingId)
{
    auto it = playingEffects.find(playingId);
    
    if (it != playingEffects.end())
    {
        int channelId = it->second.first;
        EffectRef effect = it->second.second;
        
        FMOD::Channel *channel;
        system->getChannel(channelId, &channel);
        channel->stop();
        
        playingEffects.erase(it);
        processEvent(Event(EVENT_STOPPED, effect, channelId, playingId));
        
        return true;
    }
    
    return false;
}

bool SoundEngine::stopEffects(EffectRef effect)
{
    vector<int> playingIdsToStop;
    
    for (auto &it : playingEffects)
    {
        if (it.second.second == effect)
        {
            playingIdsToStop.push_back(it.first);
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
    
    for (auto &it : playingEffects)
    {
        playingIdsToStop.push_back(it.first);
    }
    
    for (auto playingId : playingIdsToStop)
    {
        stopEffect(playingId);
    }
    
    return (!playingIdsToStop.empty());
}

bool SoundEngine::pauseEffect(int playingId)
{
    auto it = playingEffects.find(playingId);
    
    if (it != playingEffects.end())
    {
        FMOD::Channel *channel;
        system->getChannel(it->second.first, &channel);
        channel->setPaused(true);
        
        return true;
    }
    
    return false;
}

bool SoundEngine::resumeEffect(int playingId)
{
    auto it = playingEffects.find(playingId);
    
    if (it != playingEffects.end())
    {
        FMOD::Channel *channel;
        system->getChannel(it->second.first, &channel);
        channel->setPaused(false);
        
        return true;
    }
    
    return false;
}

EffectRef SoundEngine::loadEffect(InputSourceRef inputSource)
{
    FMOD_RESULT result = FMOD_OK;
    FMOD::Sound *sound = NULL;
    
    if (inputSource->isFile())
    {
        result = system->createSound(inputSource->getFilePath().c_str(), FMOD_DEFAULT, NULL, &sound);
    }
    else
    {
        auto buffer = inputSource->loadDataSource()->getBuffer();
        
        FMOD_CREATESOUNDEXINFO exinfo;
        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.length = buffer.getDataSize();
        
        result = system->createSound(static_cast<const char*>(buffer.getData()), FMOD_DEFAULT | FMOD_OPENMEMORY, &exinfo, &sound);
    }
    
    if (result)
    {
        throw FMODException(result);
    }
    else
    {
        return make_shared<Effect>(inputSource, sound);
    }
}

int SoundEngine::nextPlayingId(EffectRef effect)
{
    return ++playCount;
}

bool SoundEngine::interruptChannel(int channelId)
{
    for (auto &it : playingEffects)
    {
        if (it.second.first == channelId)
        {
            int playingId = it.first;
            EffectRef effect = it.second.second;

            playingEffects.erase(playingId);
            processEvent(Event(EVENT_INTERRUPTED, effect, channelId, playingId));
            
            return true;
        }
    }
    
    return false;
}

void SoundEngine::processEvent(const Event &event)
{
    if (listener)
    {
        listener->handleEvent(event);
    }
}
