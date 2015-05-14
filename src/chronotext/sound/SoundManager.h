/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * SoundManager IS TESTED AND DEVELOPED VIA THE TestBed2 PROJECT IN THE chronotext-playgound REPO:
 * - https://github.com/arielm/chronotext-playground/blob/master/Sketches/TestBed2/src/TestingSound1.cpp
 * - https://github.com/arielm/chronotext-playground/blob/master/Sketches/TestBed2/src/TestingSound2.cpp
 *
 * DEPENDENCIES:
 * - https://github.com/arielm/FMOD (FMOD VERSION 1.05.11)
 */

/*
 * IMPORTANT:
 *
 * - SINCE VERSION 1.03.00: "Low Level is now thread-safe by default"
 * - SOURCE: http://www.fmod.org/docs/content/generated/common/revision.html
 *
 * THIS CAN LEAD TO COUNTER-INTUITIVE SITUATIONS, E.G.:
 * - CHANNEL NOT PROPERLY PAUSED AFTER CALLING ChannelGroup::setPaused()
 *
 * SOLUTIONS:
 * - EITHER CALLING System::update() IN ORDER TO IMMEDIATLY PROCESS THE (ASYNC) COMMAND-QUEUE
 * - OR INITIALIZE WITH "FMOD_INIT_THREAD_UNSAFE" IN ORDER TO REMOVE THE COMMAND-QUEUE
 */

/*
 * FINDINGS:
 *
 * 1) OSX:
 *    - NO MEMORY SEEMS TO BE RELEASED WHEN SOUND IS DISCARDED
 *    - TESTED WITH FILE-SYSTEM + WAV
 *
 * 1) ANDROID:
 *    - NO MEMORY SEEMS TO BE RELEASED WHEN SOUND IS DISCARDED
 *    - TESTED WITH MEMORY-LOAD + WAV
 *
 * 2) IOS:
 *    - MEMORY SEEMS TO BE PROPERLY RELEASED WHEN SOUND IS DISCARDED
 *    - TESTED WITH FILE-SYSTEM + WAV
 *
 * HINT: SOME USEFUL INFO IS PRINTED WHEN USING DEBUG-VERSION OF FMOD
 */

/*
 * TODO:
 *
 * 1) MEMORY-CONSUMPTION:
 *    - OSX AND ANDROID:
 *      - TRY TO UNDERSTAND THE MEMORY MODEL(S) USED BY FMOD BEHIND THE SCENES
 *    - WHAT IS THE COST OF (NON-ENCODED AND/OR ENCODED) FILE-STREAMING?
 *    - STUDY FMOD::Memory_Initialize()
 *
 * 2) MP3 (OR ANY OTHER ENCODED FORMAT):
 *    - UNDERSTAND CPU AND MEMORY REQUIREMENTS
 *
 * 3) FIND A WAY TO ENFORCE THE CALLING OF pause() AND resume()
 *    - NECESSARY ON ANDROID, UPON FOREGROUND/BACKGROUND SWITCHES
 *    - POSSIBLE SOLUTION: chr::delegate() SHOULD ALLOW "SUBSCRIPTION TO SYSTEM SIGNALS"
 *
 * 4) ADD POSSIBILITY TO STREAM FILES (IN ADDITION TO forceMemoryLoad IN Effect::Request)
 *    - ADVANTAGEOUS FOR LARGE FILES (NO NEED TO PRELOAD EVERYTHING UP-FRONT...)
 *    - SEE IMPLEMENTATION (INCLUDING SUPPORT FOR ANDROID ASSETS) IN:
 *      https://github.com/arielm/chronotext-playground/blob/master/Sketches/TestBed2/src/TestingSound2.cpp
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/sound/Effect.h"
#include "chronotext/system/MemoryInfo.h"

#include <map>
#include <set>
#include <atomic>

namespace chr
{
    class SoundManager
    {
    public:
        static std::atomic<bool> LOG_VERBOSE;
        static std::atomic<bool> PROBE_MEMORY;
        
        // ---

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
        
        FMOD::System *system = nullptr;
        FMOD::ChannelGroup *masterGroup = nullptr;
        
        ~SoundManager();
        
        bool isValid();
        bool init(int maxChannels = 32, FMOD_INITFLAGS flags = FMOD_INIT_NORMAL);
        void uninit();
        
        /*
         * FIXME: UNSAFE
         */
        void addListener(Listener *listener);
        void removeListener(Listener *listener);
        
        /*
         * ON ANDROID (UNLIKE OTHER PLATFORMS):
         * IT IS NECESSARY TO CALL THESE UPON ACTIVITY PAUSE/RESUME
         */
        void pause();
        void resume();
        
        /*
         * IT IS MANDATORY TO CALL THIS EACH FRAME,
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

        bool pauseEffect(int playingId, int tag = -1);
        bool resumeEffect(int playingId, int tag = -1);
        bool stopEffect(int playingId, int tag = -1);
        
        bool isMute();
        void setMute(bool mute);
        
        float getVolume();
        void setVolume(float volume);

        /*
         * USE A loopCount VALUE OF -1 FOR INFINITE-LOOPING
         */
        int playEffect(Effect::Ref effect, int loopCount = 0, float volume = 1); // CAN THROW
        FMOD::Channel* playSound(FMOD::Sound *sound, int loopCount = 0, float volume = 1); // CAN THROW

        // ---
        
        /*
         * TODO: CONSIDER MOVING THE FOLLOWING TO A SEPARATE STATIC-HELPER (SIMILAR TO texture/TextureHelper)
         */
        
        struct Record
        {
            int64_t memoryUsage;
            MemoryInfo memoryInfo[1];
        };
        
        static MemoryInfo memoryInfo[1];
        static std::map<FMOD::Sound*, Record> records;
        
        static FMOD::Sound* loadSound(FMOD::System *system, const Effect::Request &request); // CAN THROW
        static double getSoundDuration(FMOD::Sound *sound);
        
        static std::string writeError(FMOD_RESULT result);
        
    protected:
        bool initialized = false;
        int effectCounter = -1;
        int playCounter = 0;

        std::map<Effect::Request, Effect::Ref> effects;
        std::map<int, std::pair<int, int>> playingEffects;
        
        std::set<Listener*> listeners;
        
        bool interruptChannel(int channelId);
        Effect::Ref findByUniqueId(int uniqueId);
        void dispatchEvent(const Event &event);
    };
}
