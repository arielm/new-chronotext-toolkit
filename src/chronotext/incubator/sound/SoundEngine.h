/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/incubator/sound/Effect.h"

#include <map>
#include <set>

class SoundEngine
{
public:
    enum Type
    {
        EVENT_UNDEFINED,
        EVENT_STARTED,
        EVENT_STOPPED, // EFFECT STOPPED BY USER
        EVENT_INTERRUPTED, // EFFECT AUTOMATICALLY STOPPED IN FAVOR OF A NEW EFFECT (I.E. WHEN NO FREE CHANNEL REMAIN)
        EVENT_COMPLETED
    };

    struct Event
    {
        Type type;
        Effect::Ref effect;
        int channelId;
        int playingId;
        
        Event()
        :
        type(EVENT_UNDEFINED),
        channelId(-1),
        playingId(0)
        {}
        
        Event(Type type, Effect::Ref effect, int channelId, int playingId)
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
                    
                case EVENT_UNDEFINED:
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
        virtual ~Listener() {}
        virtual void handleEvent(const Event &event) = 0;
    };
  
    // ---
    
    FMOD::System *system;
    FMOD::ChannelGroup *masterGroup;

    SoundEngine();
    
    void setup(int maxChannels = 32);
    void shutdown();
    
    /*
     * FIXME: UNSAFE
     */
    void addListener(Listener *listener);
    void removeListener(Listener *listener);
    
    /*
     * ON ANDROID (UNLIKE iOS):
     * IT IS NECESSARY TO CALL THESE UPON FOREGROUND/BACKGROUND SWITCHES
     */
    void pause();
    void resume();
    
    /*
     * IT IS MANDATORY TO CALL UPDATE EACH FRAME,
     * OTHERWISE (AND AMONG OTHER THINGS):
     * CHANNELS WON'T BE FREED UPON COMPLETION
     */
    void update();
    
    Effect::Ref preloadEffect(chr::InputSource::Ref inputSource); // CAN THROW
    bool unloadEffect(chr::InputSource::Ref inputSource);
    
    /*
     * THE RETURNED POINTER IS NOT INTENDED FOR STORAGE
     */
    Effect* getEffect(chr::InputSource::Ref inputSource);
    
    int playEffect(int effectId, int loopCount = 0, float volume = 1);
    bool pauseEffect(int playingId);
    bool resumeEffect(int playingId);
    bool stopEffect(int playingId);
    bool stopEffects(int effectId);
    bool stopAllEffects();

    bool isMute();
    void setMute(bool mute);

    float getVolume();
    void setVolume(float volume);
    
protected:
    std::map<std::string, Effect::Ref> effects;
    std::map<int, std::pair<int, int>> playingEffects;
    
    int playCount;
    int effectCount;
    
    std::set<Listener*> listeners;
    
    Effect* loadEffect(chr::InputSource::Ref inputSource);
    bool interruptChannel(int channelId);
    
    Event createEvent(Type type, int effectId, int channelId, int playingId);
    void dispatchEvent(const Event &event);
};
