/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * 1) FIND A WAY TO ENFORCE THE CALLING OF pause() AND resume()
 *    - NECESSARY TO CALL THESE UPON FOREGROUND/BACKGROUND SWITCHES ON ANDROID

 * 2) PROBE MEMORY, AS IN TextureHelper
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/sound/Effect.h"

#include <map>
#include <set>

namespace chr
{
    class SoundEngine
    {
    public:
        enum Type
        {
            EVENT_UNDEFINED,
            EVENT_STARTED,
            EVENT_STOPPED, // EFFECT STOPPED ON-DEMAND OR AUTOMATICALLY UPON SHUT-DOWN
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
                    
                    default:
                    return "";
                }
            }
            
            friend std::ostream& operator<<(std::ostream& lhs, const Event &rhs)
            {
                lhs <<
                "EFFECT " << rhs.description() << ": " <<
                rhs.effect->request.inputSource->getFilePathHint() <<
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
         * OTHERWISE (AND AMONG OTHER THINGS): CHANNELS WON'T BE RELEASED UPON COMPLETION
         */
        void update();
        
        Effect::Ref getEffect(const Effect::Request &request); // CAN THROW

        void discardEffect(Effect::Ref effect);
        void discardEffects();

        bool reloadEffect(Effect::Ref effect); // CAN THROW
        void reloadEffects(); // CAN THROW

        void stopEffect(Effect::Ref effect);
        void stopEffects();

        int playEffect(Effect::Ref effect, int loopCount = 0, float volume = 1); // CAN THROW

        bool pauseEffect(int playingId);
        bool resumeEffect(int playingId);
        bool stopEffect(int playingId);
        
        bool isMute();
        void setMute(bool mute);
        
        float getVolume();
        void setVolume(float volume);
        
    protected:
        FMOD::System *system;
        FMOD::ChannelGroup *masterGroup;

        std::map<Effect::Request, Effect::Ref> effects;
        std::map<int, std::pair<int, int>> playingEffects;
        
        int playCount;
        int effectCount;
        
        std::set<Listener*> listeners;
        
        bool interruptChannel(int channelId);
        
        Event createEvent(Type type, int uniqueId, int channelId, int playingId);
        void dispatchEvent(const Event &event);
        
        FMOD::Sound* createSound(const Effect::Request &request); // CAN THROW
    };
}
