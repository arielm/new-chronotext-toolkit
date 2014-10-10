/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include <jni.h>

#include "chronotext/android/cinder/CinderDelegate.h"

extern "C"
{
    /*
     * THIS IS THE "STUB" FUNCTION:
     * IT SHOULD RETURN CinderDelegate WITH A PROPERLY DEFINED CinderSketch
     *
     * TODO: ADD DEFAULT "STUB" FUNCTION (THAT COULD BE OVERRIDEN IN A NON-MANDATORY main.cpp?)
     */
    chr::CinderDelegate* createDelegate();

    void Java_org_chronotext_cinder_CinderDelegate_prelaunch(JNIEnv *env, jobject obj, jobject context, jobject listener);
    void Java_org_chronotext_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity);

    void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height);
    void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj);
    void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height);
    void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint eventId);
    void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj);
    
    void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    
    void Java_org_chronotext_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body);
}

JavaVM *gJavaVM;
chr::CinderDelegate *gDelegate;

/*
 * WARNING: THIS IS *NOT* CALLED EACH TIME THE APPLICATION STARTS
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    gJavaVM = vm;
    return JNI_VERSION_1_4;
}

/*
 * MUST BE CALLED ON THE MAIN-THREAD, BEFORE THE RENDERER'S THREAD IS CREATED
 */
void Java_org_chronotext_cinder_CinderDelegate_prelaunch(JNIEnv *env, jobject obj, jobject context, jobject listener)
{
    gDelegate = createDelegate();
    gDelegate->prelaunch(gJavaVM, env->NewGlobalRef(context), env->NewGlobalRef(listener));
}

/*
 * MUST BE CALLED ON THE RENDERER'S THREAD, BEFORE THE GL-CONTEXT IS CREATED
 */
void Java_org_chronotext_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    gDelegate->launch(env->NewGlobalRef(display), displayWidth, displayHeight, displayDensity);
}

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    gDelegate->setup(width, height);
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    gDelegate->shutdown();
    delete gDelegate;
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
        gDelegate->sendMessageToSketch(what, std::string(chars));
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        gDelegate->sendMessageToSketch(what, "");
    }
}
