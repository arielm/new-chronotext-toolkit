/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace jni
    {
        JavaVM *vm = nullptr;
        
        jobject bridge = nullptr;
        CinderDelegate *cinderDelegate = nullptr;
        
        // ---
        
        JNIEnv* getEnv()
        {
            JNIEnv *env = nullptr;
            
            auto err = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4);
            
            if (err == JNI_EDETACHED)
            {
                CI_LOGE("CURRENT THREAD NOT ATTACHED TO JAVA VM");
            }
            else if (err == JNI_EVERSION)
            {
                CI_LOGE("VM DOESN'T SUPPORT REQUESTED JNI VERSION");
            }
            
            if (env)
            {
                return env;
            }
            
            throw runtime_error("INVALID JNI ENV"); // XXX
        }
        
        // ---
        
        string toString(jstring s)
        {
            if (s)
            {
                JNIEnv *env = getEnv();
                
                const char *chars = env->GetStringUTFChars(s, nullptr);
                
                if (chars)
                {
                    string tmp(chars);
                    env->ReleaseStringUTFChars(s, chars);
                    return tmp;
                }
            }
            
            return "";
        }
        
        jstring toJString(const string &s)
        {
            return getEnv()->NewStringUTF(s.data());
        }
        
        // ---
        
        /*
         * TODO: THROW "JNI EXCEPTION" WITH THE RELEVANT JAVA ERROR MESSAGE
         *
         * - IF METHOD CAN'T BE FOUND
         * - IF CALL FAILED
         *
         * HINTS: http://stackoverflow.com/questions/6015293/convert-exceptiondescribe-to-string
         */
        
        void callVoidMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            env->CallVoidMethodV(bridge, method, args);
            va_end(args);
        }
        
        jboolean callBooleanMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jboolean ret = env->CallBooleanMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jchar callCharMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jchar ret = env->CallCharMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jint callIntMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jint ret = env->CallIntMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jlong callLongMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jlong ret = env->CallLongMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jfloat callFloatMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jfloat ret = env->CallFloatMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jdouble callDoubleMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jdouble ret = env->CallDoubleMethod(bridge, method, args);
            va_end(args);
            
            return ret;
        }
        
        jobject callObjectMethodOnBridge(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(bridge);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jobject ret = env->CallObjectMethodV(bridge, method, args);
            va_end(args);
            
            return ret;
        }
    }
}

#pragma mark ---------------------------------------- JAVA <-> C++ ----------------------------------------

using namespace chr;

/*
 * WARNING: THIS IS *NOT* NECESSARILY CALLED EACH TIME THE APPLICATION STARTS...
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    CI_LOGV("ONLOAD");
    
    jni::vm = vm; // ...THEREFORE: THIS SHOULD *NOT* BE CLEARED
    
    return JNI_VERSION_1_4;
}

void Java_org_chronotext_cinder_CinderBridge_init(JNIEnv *env, jobject obj, jobject bridge, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    jni::bridge = env->NewGlobalRef(bridge);
    
    jni::cinderDelegate = new CinderDelegate();
    jni::cinderDelegate->performInit(env, context, display, Vec2i(displayWidth, displayHeight), displayDensity);
}

void Java_org_chronotext_cinder_CinderBridge_uninit(JNIEnv *env, jobject obj)
{
    jni::cinderDelegate->performUninit(env);
    delete jni::cinderDelegate;
    
    env->DeleteGlobalRef(jni::bridge);
    jni::bridge = nullptr;
    
    CI_LOGV("UNINIT");
}

void Java_org_chronotext_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height)
{
    jni::cinderDelegate->performSetup(env, Vec2i(width, height));
}

void Java_org_chronotext_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    jni::cinderDelegate->performShutdown(env);
}

// ---

void Java_org_chronotext_cinder_CinderRenderer_resize(JNIEnv *env, jobject obj, jint width, jint height)
{
    jni::cinderDelegate->performResize(Vec2i(width, height));
}

void Java_org_chronotext_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj)
{
    jni::cinderDelegate->performUpdate();
    jni::cinderDelegate->performDraw();
}

// ---

void Java_org_chronotext_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    jni::cinderDelegate->addTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    jni::cinderDelegate->updateTouch(index, x, y);
}

void Java_org_chronotext_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jint index, jfloat x, jfloat y)
{
    jni::cinderDelegate->removeTouch(index, x, y);
}

// ---

void Java_org_chronotext_cinder_CinderRenderer_dispatchEvent(JNIEnv *env, jobject obj, jint eventId)
{
    jni::cinderDelegate->handleEvent(eventId);
}

void Java_org_chronotext_cinder_CinderBridge_sendMessageToSketch(JNIEnv *env, jobject obj, jint what, jstring body)
{
    if (body)
    {
        const char *chars = env->GetStringUTFChars(body, nullptr);
        jni::cinderDelegate->messageFromBridge(what, chars);
        env->ReleaseStringUTFChars(body, chars);
    }
    else
    {
        jni::cinderDelegate->messageFromBridge(what);
    }
}
