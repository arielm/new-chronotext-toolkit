#pragma once

#include "Effect.h"

#include <map>

class SoundEngine
{
public:
    enum
    {
        EVENT_UNDEFINED,
        EVENT_STARTED,
        EVENT_STOPPED,
        EVENT_INTERRUPTED,
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
                    return  "COMPLETED";
                    
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
    
    void pause();
    void resume();
    void update();
    
    EffectRef preloadEffect(InputSourceRef inputSource);
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
    
    EffectRef loadEffect(InputSourceRef inputSource);
    int nextPlayingId(EffectRef effect);
    bool interruptChannel(int channelId);
    void processEvent(const Event &event);
};
