/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "JNI.h"

#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/Context.h"

using namespace std;

namespace chr
{
    namespace jni
    {
        JavaVM *vm = nullptr;
        
        // ---
        
        JNIEnv* env()
        {
            JNIEnv *env_ = nullptr;
            
            auto err = vm->GetEnv(reinterpret_cast<void**>(&env_), JNI_VERSION_1_4);
            
            if (err == JNI_EDETACHED)
            {
                CI_LOGE("CURRENT THREAD NOT ATTACHED TO JAVA VM");
            }
            else if (err == JNI_EVERSION)
            {
                CI_LOGE("VM DOESN'T SUPPORT REQUESTED JNI VERSION");
            }
            
            if (env_)
            {
                return env_;
            }
            
            throw runtime_error("INVALID JNI ENV");
        }
        
        string toString(jstring s)
        {
            if (s)
            {
                JNIEnv *env_ = env();
                
                const char *chars = env_->GetStringUTFChars(s, nullptr);
                
                if (chars)
                {
                    string tmp(chars);
                    env_->ReleaseStringUTFChars(s, chars);
                    return tmp;
                }
            }
            
            return "";
        }
    }
    
    namespace CONTEXT
    {
        CinderDelegate *delegate = nullptr;
    }
}

#pragma mark ---------------------------------------- JNI ----------------------------------------

using namespace chr;

/*
 * WARNING: THIS IS *NOT* NECESSARILY CALLED EACH TIME THE APPLICATION STARTS
 */

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    CI_LOGI("ONLOAD");
    
    jni::vm = vm;
    return JNI_VERSION_1_4;
}

/*
 * MUST BE CALLED ON THE MAIN-THREAD, BEFORE THE RENDERER'S THREAD IS CREATED
 */

void Java_org_chronotext_cinder_CinderDelegate_init(JNIEnv *env, jobject obj, jobject context, jobject listener, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    CONTEXT::delegate = new CinderDelegate();
    CONTEXT::delegate->init(env, env->NewGlobalRef(context), env->NewGlobalRef(listener), env->NewGlobalRef(display), displayWidth, displayHeight, displayDensity);
}

/*
 * MUST BE CALLED ON THE RENDERER'S THREAD, AFTER GL-CONTEXT IS CREATED
 */

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    CONTEXT::delegate->setup(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    CONTEXT::delegate->shutdown();
    
    delete CONTEXT::delegate;
    CONTEXT::delegate = nullptr;
    
    CI_LOGI("SHUTDOWN");
}

// ---

void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
    CONTEXT::delegate->resize(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj)
{
    CONTEXT::delegate->draw();
}

// ---

void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint eventId)
{
    CONTEXT::delegate->event(eventId);
}

void Java_org_chronotext_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body)
{
    if (body)
    {
        const char *chars = env->GetStringUTFChars(body, nullptr);
        CONTEXT::delegate->sendMessageToSketch(what, chars);
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        CONTEXT::delegate->sendMessageToSketch(what);
    }
}

// ---

void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    CONTEXT::delegate->addTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    CONTEXT::delegate->updateTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    CONTEXT::delegate->removeTouch(index, x, y);
}
