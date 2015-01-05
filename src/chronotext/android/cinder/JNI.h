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
 * 1) CHECK IF IT'S POSSIBLE TO MODERNIZE JNI USAGE:
 *    - E.G. USING "METHOD TABLES":
 *      - http://sbcgamesdev.blogspot.co.il/2012/12/using-jnionload-in-adroid-ndk.html
 *    - MORE JNI WISDOM HERE: http://developer.android.com/training/articles/perf-jni.html
 */

#pragma once

#include <jni.h>

#include <string>

extern "C"
{
    jint JNI_OnLoad(JavaVM *vm, void *reserved);
    
    // ---
    
    void Java_org_chronotext_cinder_CinderDelegate_init(JNIEnv *env, jobject obj, jobject listener, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity);

    void Java_org_chronotext_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj);
    void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height);
    void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj);
    
    void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height);
    void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj);
    
    void Java_org_chronotext_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint eventId);
    void Java_org_chronotext_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body);
    
    void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
    void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y);
}

// ---

namespace chr
{
    namespace jni
    {
        extern JavaVM *vm;
        extern jobject listener;
        
        // ---
        
        JNIEnv* env();
        
        std::string toString(jstring s);
        jstring toJString(const std::string &s);
    }
    
    // ---
    
    class CinderDelegate;
    
    namespace CONTEXT
    {
        extern CinderDelegate *delegate;
    }
}
