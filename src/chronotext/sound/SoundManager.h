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
 *    - NECESSARY ON ANDROID, UPON FOREGROUND/BACKGROUND SWITCHES

 * 2) PROBE MEMORY (AND MORE...)
 *    - AS IN TextureHelper AND Texture
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/sound/Effect.h"

#include <map>
#include <set>
#include <atomic>

namespace chr
{
    class SoundManager
    {
    public:
        static std::atomic<bool> LOG_VERBOSE;

        enum Type
        {
            EVENT_STARTED,
            EVENT_STOPPED, // EFFECT STOPPED ON-DEMAND OR AUTOMATICALLY UPON SHUT-DOWN
            EVENT_INTERRUPTED, // EFFECT STOPPED AUTOMATICALLY IN FAVOR OF A NEW EFFECT (I.E. WHEN NO FREE-CHANNEL REMAIN)
            EVENT_COMPLETED
        };
        
        struct Event
        {
            Type type;
            Effect::Ref effect;
            int channelId;
            int playingId;
            
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
        
        SoundManager();
        ~SoundManager();
        
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
        Effect::Ref findEffect(const Effect::Request &request) const;
        
        void discardEffect(Effect::Ref effect);
        bool reloadEffect(Effect::Ref effect); // CAN THROW

        void discardEffects(int tag = -1);
        void reloadEffects(int tag = -1); // CAN THROW

        void stopEffect(Effect::Ref effect);
        void stopEffects(int tag = -1);

        int playEffect(Effect::Ref effect, int loopCount = 0, float volume = 1); // CAN THROW

        bool pauseEffect(int playingId, int tag = -1);
        bool resumeEffect(int playingId, int tag = -1);
        bool stopEffect(int playingId, int tag = -1);
        
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
        Effect::Ref findByUniqueId(int uniqueId);
        void dispatchEvent(const Event &event);
        
        FMOD::Sound* createSound(const Effect::Request &request); // CAN THROW
    };
}
