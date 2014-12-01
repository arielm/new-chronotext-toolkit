/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"

#include <android/log.h>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "cinder", __VA_ARGS__))

using namespace std;
using namespace chr;

namespace chronotext
{
    namespace java
    {
        JavaVM *vm = nullptr;
        
        JNIEnv* getJNIEnv()
        {
            JNIEnv *env = nullptr;
            
            auto err = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4);
            
            if (err == JNI_EDETACHED)
            {
                LOGE("CURRENT THREAD NOT ATTACHED TO JAVA VM");
            }
            else if (err == JNI_EVERSION)
            {
                LOGE("VM DOESN'T SUPPORT REQUESTED JNI VERSION");
            }
            
            if (env)
            {
                return env;
            }
            
            throw runtime_error("INVALID JNI ENV");
        }
    }
}

/*
 * WARNING: THIS IS *NOT* CALLED EACH TIME THE APPLICATION STARTS
 */

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    java::vm = vm;
    return JNI_VERSION_1_4;
}

#pragma mark ---------------------------------------- NATIVE METHODS ----------------------------------------

chr::CinderDelegate *gDelegate = nullptr; // TODO: USE NAMESPACE

/*
 * MUST BE CALLED ON THE MAIN-THREAD, BEFORE THE RENDERER'S THREAD IS CREATED
 */

void Java_org_chronotext_cinder_CinderDelegate_prelaunch(JNIEnv *env, jobject obj, jobject context, jobject listener, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    gDelegate = new CinderDelegate();
    gDelegate->prelaunch(env, env->NewGlobalRef(context), env->NewGlobalRef(listener), env->NewGlobalRef(display), displayWidth, displayHeight, displayDensity);
}

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    gDelegate->setup(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    gDelegate->shutdown();
    
    delete gDelegate;
    gDelegate = nullptr;
}

void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
    gDelegate->resize(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint eventId)
{
    gDelegate->event(eventId);
}

void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj)
{
    gDelegate->draw();
}

void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    gDelegate->addTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    gDelegate->updateTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    gDelegate->removeTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body)
{
    if (body)
    {
        const char *chars = env->GetStringUTFChars(body, nullptr);
        gDelegate->sendMessageToSketch(what, chars);
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        gDelegate->sendMessageToSketch(what);
    }
}

#undef LOGE
