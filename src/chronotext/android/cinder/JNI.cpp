/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"

using namespace std;
using namespace chr;

namespace chronotext
{
    namespace jvm
    {
        JavaVM *vm = nullptr;
        
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
    }
    
    namespace context
    {
        CinderDelegate *delegate = nullptr;
    }
}

/*
 * WARNING: THIS IS *NOT* CALLED EACH TIME THE APPLICATION STARTS
 */

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jvm::vm = vm;
    return JNI_VERSION_1_4;
}

#pragma mark ---------------------------------------- NATIVE METHODS ----------------------------------------

/*
 * MUST BE CALLED ON THE MAIN-THREAD, BEFORE THE RENDERER'S THREAD IS CREATED
 */

void Java_org_chronotext_cinder_CinderDelegate_prelaunch(JNIEnv *env, jobject obj, jobject context, jobject listener, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    context::delegate = new CinderDelegate();
    context::delegate->prelaunch(env, env->NewGlobalRef(context), env->NewGlobalRef(listener), env->NewGlobalRef(display), displayWidth, displayHeight, displayDensity);
}

/*
 * MUST BE CALLED ON THE RENDERER'S THREAD, AFTER GL-CONTEXT IS CREATED
 */

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    context::delegate->setup(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    context::delegate->shutdown();
    
    delete context::delegate;
    context::delegate = nullptr;
    
    CI_LOGI("SHUTDOWN");
}

void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
    context::delegate->resize(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint eventId)
{
    context::delegate->event(eventId);
}

void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj)
{
    context::delegate->draw();
}

void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    context::delegate->addTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    context::delegate->updateTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    context::delegate->removeTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body)
{
    if (body)
    {
        const char *chars = env->GetStringUTFChars(body, nullptr);
        context::delegate->sendMessageToSketch(what, chars);
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        context::delegate->sendMessageToSketch(what);
    }
}

#undef LOGE
