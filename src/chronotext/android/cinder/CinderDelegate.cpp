/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace chr
{
    atomic<bool> CinderDelegate::LOG_VERBOSE (false);
    atomic<bool> CinderDelegate::LOG_WARNING (true);
    
    namespace intern
    {
        CinderDelegate *instance = nullptr;
    }
    
    CinderDelegate& delegate()
    {
        return checkedReference(intern::instance);
    }
    
    // ---
    
    bool CinderDelegate::performInit(JNIEnv *env, jobject androidContext, jobject androidDisplay, const Vec2i &displaySize, float displayDensity)
    {
        if (!initialized_)
        {
            initInfo.androidContext = env->NewGlobalRef(androidContext);
            initInfo.androidDisplay = env->NewGlobalRef(androidDisplay);
            initInfo.displaySize = displaySize;
            initInfo.displayDensity = displayDensity;

            intern::instance = this;
            initialized_ = _init();
        }
        
        return initialized_;
    }
    
    void CinderDelegate::performUninit(JNIEnv *env)
    {
        if (initialized_ && !setup_)
        {
            _uninit();
            
            env->DeleteGlobalRef(initInfo.androidContext);
            env->DeleteGlobalRef(initInfo.androidDisplay);
            
            // ---

            initialized_ = false;
            intern::instance = nullptr;
        }
    }
    
    void CinderDelegate::performSetup(JNIEnv *env, const Vec2i &size)
    {
        if (!setup_ && initialized_)
        {
            startIOService();
            createSensorEventQueue();
            
            setupInfo.io_service = io.get();
            setupInfo.windowInfo.size = size;
            
            _setup();
            
            // ---
            
            setup_ = true;
        }
    }
    
    void CinderDelegate::performShutdown(JNIEnv *env)
    {
        if (setup_)
        {
            /*
             * TODO:
             *
             * - HANDLE PROPERLY THE SHUTING-DOWN OF "UNDERGOING" TASKS
             * - SEE RELATED TODOS IN CinderDelegateBase AND TaskManager
             */
            _shutdown();
            
            destroySensorEventQueue();
            stopIOService();
            
            // ---
            
            setup_ = false;
        }
    }
    
    void CinderDelegate::performResize(const Vec2i &size)
    {
        assert(setup_);
        
        setupInfo.windowInfo.size = size;
        sketch->performResize(size);
    }
    
    void CinderDelegate::performUpdate()
    {
        assert(setup_);
        
        /*
         * SHOULD TAKE PLACE BEFORE IO-SERVICE-POLLING
         *
         * SUBSEQUENT CALLS TO FrameClock::getTime() DURING THE FRAME WILL RETURN THE SAME TIME-SAMPLE
         */
        sketch->clock()->update(true);
        
        pollSensorEvents(); // WHERE handleAcceleration IS INVOKED
        io->poll(); // WHERE addTouch, updateTouch, removeTouch, ETC. ARE INVOKED
        
        sketch->performUpdate();
        updateCount++;
    }
    
    void CinderDelegate::performDraw()
    {
        assert(setup_);
        sketch->draw();
    }
    
#pragma mark ---------------------------------------- SKETCH <-> BRIDGE COMMUNICATION ----------------------------------------
    
    /*
     * TODO: FORMAT body FOR LOG
     *
     * 1) LEADING AND TRAILING WHITE-SPACE TRIMMED
     * 2) LINE-BREAKS AND TABS REPLACED BY SPACES
     * 3) TEXT-LENGTH LIMITED
     */
    
    /*
     * WILL BE QUEUED TO THE RENDERER'S THREAD (VIA CPP-HANDLER)
     */
    void CinderDelegate::messageFromBridge(int what, const string &body)
    {
        LOGI_IF(LOG_VERBOSE) << "MESSAGE RECEIVED FROM BRIDGE: " << what << " " << body << endl;
        
        CinderDelegateBase::messageFromBridge(what, body);
    }
    
    /*
     * WILL BE QUEUED TO THE MAIN-THREAD THREAD (VIA JAVA-HANDLER)
     */
    void CinderDelegate::sendMessageToBridge(int what, const string &body)
    {
        LOGI_IF(LOG_VERBOSE) << "MESSAGE SENT TO BRIDGE: " << what << " " << body << endl;
        
        jni::callVoidMethodOnBridge("messageFromSketch", "(ILjava/lang/String;)V", what, jni::toJString(body));
    }

    void CinderDelegate::handleEvent(int eventId)
    {
        switch (eventId)
        {
            case CinderSketch::EVENT_RESUMED:
                sketch->performStart(CinderSketch::START_REASON_APP_RESUMED);
                return;
                
            case CinderSketch::EVENT_SHOWN:
                sketch->performStart(CinderSketch::START_REASON_VIEW_SHOWN);
                return;
                
            case CinderSketch::EVENT_PAUSED:
                sketch->performStop(CinderSketch::STOP_REASON_APP_PAUSED);
                return;
                
            case CinderSketch::EVENT_HIDDEN:
                sketch->performStop(CinderSketch::STOP_REASON_VIEW_HIDDEN);
                return;
        }
        
        sketch->event(eventId);
    }
    
    /*
     * TODO: FINALIZE THREAD-SAFETY POLICY
     */
    void CinderDelegate::performAction(int actionId)
    {
        jni::callBooleanMethodOnBridge("handleAction", "(I)Z", actionId);
    }
    
    // ---
    
    /*
     * CURRENT LIMITATION: MUST BE CALLED FROM THE MAIN-THREAD OR THE RENDERER'S THREAD
     *
     * TODO:
     *
     * 1) ADD SUPPORT FOR JAVA-THREAD-ATTACHMENT IN os/Task
     * 2) FINALIZE THREAD-SAFETY POLICY
     * 3) HANDLE POTENTIAL JNI EXCEPTIONS IN callObjectMethodOnBridge()
     */
    
    JsonTree CinderDelegate::jsonQuery(const char *methodName)
    {
        const string &query = jni::toString((jstring)jni::callObjectMethodOnBridge(methodName, "()Ljava/lang/String;"));
        
        if (!query.empty())
        {
            try
            {
                return JsonTree(query);
            }
            catch (exception &e)
            {
                LOGI_IF(LOG_WARNING)  << "JSON-QUERY FAILED | REASON: " << e.what() << endl;
            }
        }
        
        return JsonTree();
    }
    
#pragma mark ---------------------------------------- IO-SERVICE ----------------------------------------
    
    void CinderDelegate::startIOService()
    {
        if (!io)
        {
            io = make_shared<boost::asio::io_service>();
            ioWork = make_shared<boost::asio::io_service::work>(*io);
        }
    }
    
    void CinderDelegate::stopIOService()
    {
        if (io)
        {
            io->stop();
            
            ioWork.reset();
            io.reset();
        }
    }
    
#pragma mark ---------------------------------------- TOUCH ----------------------------------------
    
    void CinderDelegate::addTouch(int index, float x, float y)
    {
        sketch->addTouch(index, x, y);
    }
    
    void CinderDelegate::updateTouch(int index, float x, float y)
    {
        sketch->updateTouch(index, x, y);
    }
    
    void CinderDelegate::removeTouch(int index, float x, float y)
    {
        sketch->removeTouch(index, x, y);
    }
    
    
#pragma mark ---------------------------------------- KEYBOARD ----------------------------------------
    
    int CinderDelegateBase::getCode(const KeyEvent &keyEvent)
    {
        return 0;
    }
    
    bool CinderDelegateBase::isShiftDown(const KeyEvent &keyEvent)
    {
        return false;
    }
    
    bool CinderDelegateBase::isAltDown(const KeyEvent &keyEvent)
    {
        return false;
    }
    
    bool CinderDelegateBase::isAccelDown(const KeyEvent &keyEvent)
    {
        return false;
    }
    
#pragma mark ---------------------------------------- ACCELEROMETER ----------------------------------------
    
    /*
     * REFERENCES:
     *
     * http://android-developers.blogspot.co.il/2010/09/one-screen-turn-deserves-another.html
     * http://developer.download.nvidia.com/tegra/docs/tegra_android_accelerometer_v5f.pdf
     *
     * PROBABLY OVERKILL...
     */
    static void canonicalToWorld(int displayRotation, float *canVec, ci::Vec3f &worldVec)
    {
        struct AxisSwap
        {
            int negateX;
            int negateY;
            int xSrc;
            int ySrc;
        };
        
        static const AxisSwap axisSwap[] =
        {
            { 1,  1, 0, 1 }, // ROTATION_0
            {-1,  1, 1, 0 }, // ROTATION_90
            {-1, -1, 0, 1 }, // ROTATION_180
            { 1, -1, 1, 0 }  // ROTATION_270
        };
        
        const AxisSwap &as = axisSwap[displayRotation];
        
        worldVec.x = as.negateX * canVec[as.xSrc];
        worldVec.y = as.negateY * canVec[as.ySrc];
        worldVec.z = canVec[2];
    }
    
    void CinderDelegate::enableAccelerometer(float updateFrequency, float filterFactor)
    {
        accelFilter = AccelEvent::Filter(filterFactor);
        
        int delay = 1000000 / updateFrequency;
        int min = ASensor_getMinDelay(accelerometerSensor);
        
        if (delay < min)
        {
            delay = min;
        }
        
        ASensorEventQueue_enableSensor(sensorEventQueue, accelerometerSensor);
        ASensorEventQueue_setEventRate(sensorEventQueue, accelerometerSensor, delay);
    }
    
    void CinderDelegate::disableAccelerometer()
    {
        ASensorEventQueue_disableSensor(sensorEventQueue, accelerometerSensor);
    }
    
    void CinderDelegate::handleAcceleration(ASensorEvent event)
    {
        int displayRotation = getDisplayRotation();
        
        Vec3f transformed;
        canonicalToWorld(displayRotation, (float*)&event.acceleration.v, transformed);
        
        /*
         * ADDITIONAL TRANSFORMATION: FOR CONSISTENCY WITH iOS
         */
        transformed *= Vec3f(-1, -1, +1) / ASENSOR_STANDARD_GRAVITY; // TODO: DOUBLE-CHECK Z AXIS
        
        sketch->accelerated(accelFilter.process(transformed));
    }
    
    /*
     * TODO: CONSIDER HANDLING IN DisplayHelper
     */
    int CinderDelegate::getDisplayRotation()
    {
        JNIEnv *env = jni::getEnv();
        
        jmethodID getRotationMethod = env->GetMethodID(env->GetObjectClass(initInfo.androidDisplay), "getRotation", "()I");
        return env->CallIntMethod(initInfo.androidDisplay, getRotationMethod);
    }
    
#pragma mark ---------------------------------------- SENSOR EVENTS ----------------------------------------
    
    void CinderDelegate::createSensorEventQueue()
    {
        auto looper = ALooper_forThread();
        
        if (!looper)
        {
            looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        }
        
        sensorManager = ASensorManager_getInstance();
        accelerometerSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
        sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 3, nullptr, nullptr);
    }
    
    void CinderDelegate::destroySensorEventQueue()
    {
        ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
    }
    
    void CinderDelegate::pollSensorEvents()
    {
        ASensorEvent event;
        
        while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0)
        {
            if (event.type == ASENSOR_TYPE_ACCELEROMETER)
            {
                handleAcceleration(event);
            }
        }
    }
}
