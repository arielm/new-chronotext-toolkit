/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
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
     * IT SHOULD RETURN CinderDelegate WITH A
     * PROPERLY DEFINED CinderSketch
     */
    chr::CinderDelegate* createDelegate();

    void Java_org_chronotext_cinder_CinderRenderer_prelaunch(JNIEnv *env, jobject obj);
    void Java_org_chronotext_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj, jobject context, jobject listener);

    void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height, jint accelerometerRotation);
    void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj);

    void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint id);
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
 * THIS MUST BE CALLED AS SOON AS THE APP STARTS
 */
void Java_org_chronotext_cinder_CinderRenderer_prelaunch(JNIEnv *env, jobject obj)
{
    gDelegate = createDelegate();
}

/*
 * THIS MUST BE CALLED FROM THE RENDERER'S THREAD
 */
void Java_org_chronotext_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj, jobject context, jobject listener)
{
    /*
     * FROM ICS AND HIGHER, WE NEED TO WRAP jobject INSTANCES AS A GLOBAL-REFERENCES
     * IN THEORY, WE SHOULD DELETE THE REFERENCES UPON APPLICATION-EXIT
     * (BUT IN PRACTICE, THIS IS A NON-ISSUE...)
     */
    gDelegate->launch(gJavaVM, env->NewGlobalRef(context), env->NewGlobalRef(listener));
}

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height, jint accelerometerRotation)
{
    gDelegate->setup(width, height, accelerometerRotation);
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    gDelegate->shutdown();
    delete gDelegate;
}

void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint id)
{
    gDelegate->event(id);
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
    if (body == NULL)
    {
        gDelegate->sendMessageToSketch(what, "");
    }
    else
    {
        const char* chars = env->GetStringUTFChars(body, NULL);
        gDelegate->sendMessageToSketch(what, std::string(chars));
        env->ReleaseStringUTFChars(body, chars);
    }
}
