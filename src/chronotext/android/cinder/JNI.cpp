/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/cinder/JNI.h"
#include "chronotext/android/cinder/CinderDelegate.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace jni
    {
        JavaVM *vm = nullptr;
        jobject listener = nullptr;
        
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
         * CURRENT LIMITATION: MUST BE CALLED FROM THE MAIN-THREAD OR THE RENDERER'S THREAD
         *
         * TODO:
         *
         * 1) ADD SUPPORT FOR JAVA-THREAD-ATTACHMENT IN os/Task
         * 2) ADD THREAD-LOCK
         */
        
        JsonTree jsonQuery(const char *methodName)
        {
            const string &query = toString((jstring)callObjectMethodOnListener(methodName, "()Ljava/lang/String;"));
            
            if (!query.empty())
            {
                try
                {
                    return JsonTree(query);
                }
                catch (exception &e)
                {
                    LOGI_IF(CinderDelegate::LOG_WARNING)  << "JSON-QUERY FAILED | REASON: " << e.what() << endl;
                }
            }
            
            return JsonTree();
        }
        
        // ---
        
        void callVoidMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            env->CallVoidMethodV(listener, method, args);
            va_end(args);
        }
        
        jboolean callBooleanMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jboolean ret = env->CallBooleanMethodV(listener, method, args);
            va_end(args);
            
            return ret;
        }
        
        jchar callCharMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jchar ret = env->CallCharMethodV(listener, method, args);
            va_end(args);
            
            return ret;
        }
        
        jint callIntMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jint ret = env->CallIntMethodV(listener, method, args);
            va_end(args);
            
            return ret;
        }
        
        jlong callLongMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jlong ret = env->CallLongMethodV(listener, method, args);
            va_end(args);
            
            return ret;
        }
        
        jfloat callFloatMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jfloat ret = env->CallFloatMethodV(listener, method, args);
            va_end(args);
            
            return ret;
        }
        
        jdouble callDoubleMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jdouble ret = env->CallDoubleMethod(listener, method, args);
            va_end(args);
            
            return ret;
        }
        
        jobject callObjectMethodOnListener(const char *name, const char *sig, ...)
        {
            JNIEnv *env = getEnv();
            
            jclass cls = env->GetObjectClass(listener);
            jmethodID method = env->GetMethodID(cls, name, sig);
            
            va_list args;
            va_start(args, sig);
            jobject ret = env->CallObjectMethodV(listener, method, args);
            va_end(args);
            
            return ret;
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
 * MUST BE CALLED ON THE MAIN-THREAD, BEFORE RENDERER'S THREAD IS CREATED
 */
void Java_org_chronotext_cinder_CinderDelegate_init(JNIEnv *env, jobject obj, jobject listener, jobject context, jobject display, jint displayWidth, jint displayHeight, jfloat displayDensity)
{
    jni::listener = env->NewGlobalRef(listener);
    
    CONTEXT::delegate = new CinderDelegate();
    CONTEXT::delegate->init(env->NewGlobalRef(context), env->NewGlobalRef(display), displayWidth, displayHeight, displayDensity);
}

/*
 * MUST BE CALLED ON THE RENDERER'S THREAD, BEFORE GL-CONTEXT IS CREATED
 */
void Java_org_chronotext_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj)
{
    CONTEXT::delegate->launch();
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
