/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"

namespace chronotext
{
    namespace system
    {
        JavaVM *vm = nullptr;
    }
}

chr::CinderDelegate *delegate; // XXX

/*
 * WARNING: THIS IS *NOT* CALLED EACH TIME THE APPLICATION STARTS
 */

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    chronotext::system::vm = vm;
    return JNI_VERSION_1_4;
}

/*
 * MUST BE CALLED ON THE MAIN-THREAD, BEFORE THE RENDERER'S THREAD IS CREATED
 */

void Java_org_chronotext_cinder_CinderDelegate_prelaunch(JNIEnv *env, jobject obj, jobject context, jobject listener, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    delegate = new chr::CinderDelegate();
    delegate->prelaunch(env, env->NewGlobalRef(context), env->NewGlobalRef(listener), env->NewGlobalRef(display), displayWidth, displayHeight, displayDensity);
}

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    delegate->setup(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    delegate->shutdown();
    
    delete delegate;
    delegate = nullptr;
}

void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
    delegate->resize(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint eventId)
{
    delegate->event(eventId);
}

void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj)
{
    delegate->draw();
}

void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    delegate->addTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    delegate->updateTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    delegate->removeTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body)
{
    if (body)
    {
        const char *chars = env->GetStringUTFChars(body, nullptr);
        delegate->sendMessageToSketch(what, chars);
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        delegate->sendMessageToSketch(what);
    }
}
