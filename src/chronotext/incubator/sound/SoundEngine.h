/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/incubator/sound/Effect.h"

#include <map>

class SoundEngine
{
public:
    enum
    {
        EVENT_UNDEFINED,
        EVENT_STARTED,
        EVENT_STOPPED, // EFFECT STOPPED BY USER
        EVENT_INTERRUPTED, // EFFECT AUTOMATICALLY STOPPED IN FAVOR OF A NEW EFFECT (I.E. WHEN NO FREE CHANNEL REMAIN)
        EVENT_COMPLETED
    };
    
    struct Event
    {
        int type;
        EffectRef effect;
        int channelId;
        int playingId;
        
        Event()
        :
        type(EVENT_UNDEFINED),
        channelId(-1),
        playingId(0)
        {}
        
        Event(int type, EffectRef effect, int channelId, int playingId)
        :
        type(type),
        effect(effect),
        channelId(channelId),
        playingId(playingId)
        {}
        
        std::string description() const
        {
            switch (type)
            {
                case EVENT_STARTED:
                    return "STARTED";
                    
                case EVENT_STOPPED:
                    return "STOPPED";
                    
                case EVENT_INTERRUPTED:
                    return "INTERRUPTED";
                    
                case EVENT_COMPLETED:
                    return "COMPLETED";
                    
                default:
                    return "";
            }
        }
        
        friend std::ostream& operator<<(std::ostream& lhs, const Event &rhs)
        {
            lhs <<
            "EFFECT " << rhs.description() << ": " <<
            rhs.effect->inputSource->getFilePathHint() <<
            " | CHANNEL: " << rhs.channelId <<
            " | ID: " << rhs.playingId;
            
            return lhs;
        }
    };

    class Listener
    {
    public:
        virtual void handleEvent(const Event &event) = 0;
    };
  
    SoundEngine();
    
    void setup(int maxChannels = 32);
    void shutdown();
            
    void setListener(Listener *listener);
    
    /*
     * NO NEED TO CALL THESE ON MOBILE DEVICES UPON
     * FOREGROUND/BACKGROUND SWITCHES
     * (THIS IS HANDLED BY FMOD AUTOMATICALLY...)
     */
    void pause();
    void resume();
    
    /*
     * IT IS MANDATORY TO CALL UPDATE UPON EACH FRAME
     * OTHERWISE (AND AMONG OTHER THINGS):
     * CHANNELS WON'T BE FREED UPON COMPLETION
     */
    void update();
    
    EffectRef preloadEffect(chr::InputSourceRef inputSource);
    void unloadEffect(EffectRef effect);
    
    int playEffect(EffectRef effect, int loopCount = 0, float volume = 1);
    bool stopEffect(int playingId);
    bool stopEffects(EffectRef effect);
    bool stopAllEffects();

    void setMute(bool mute);
    void setVolume(float volume);
    
protected:
    FMOD::System *system;
    FMOD::ChannelGroup *masterGroup;
    
    std::map<std::string, EffectRef> effects;
    std::map<int, std::pair<int, EffectRef>> playingEffects;
    
    int playCount;
    Listener *listener;
    
    EffectRef loadEffect(chr::InputSourceRef inputSource);
    int nextPlayingId(EffectRef effect);
    bool interruptChannel(int channelId);
    void processEvent(const Event &event);
};
