#include <jni.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "chronotext/android/cinder/CinderDelegate.h"

extern "C"
{
    /*
     * THIS IS THE "STUB" FUNCTION:
     * IT SHOULD RETURN CinderDelegate WITH A
     * PROPERLY DEFINED CinderSketch
     */
    CinderDelegate* createDelegate();

    void Java_chronotext_android_cinder_CinderRenderer_prelaunch(JNIEnv *env, jobject obj);
	void Java_chronotext_android_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj, jobject assetManager, jobject listener);

	void Java_chronotext_android_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height, jint accelerometerRotation);
	void Java_chronotext_android_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj);

	void Java_chronotext_android_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint id);
	void Java_chronotext_android_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj);
    
	void Java_chronotext_android_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jfloat x, jfloat y);
	void Java_chronotext_android_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jfloat x, jfloat y);
	void Java_chronotext_android_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jfloat x, jfloat y);
    
	void Java_chronotext_android_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body);
}

JavaVM *gJavaVM;
CinderDelegate *gDelegate;

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
void Java_chronotext_android_cinder_CinderRenderer_prelaunch(JNIEnv *env, jobject obj)
{
    gDelegate = createDelegate();
}

/*
 * THIS MUST BE CALLED FROM THE RENDERER'S THREAD
 */
void Java_chronotext_android_cinder_CinderRenderer_launch(JNIEnv *env, jobject obj, jobject assetManager, jobject listener)
{
	/*
	 * FROM ICS AND HIGHER, WE NEED TO WRAP listener AS A GLOBAL-REFERENCE
	 * IN THEORY, WE SHOULD DELETE THE REFERENCE UPON APPLICATION-EXIT
	 * (BUT IN PRACTICE, THIS IS A NON-ISSUE...)
	 */
	gDelegate->launch(AAssetManager_fromJava(env, assetManager), gJavaVM, env->NewGlobalRef(listener));
}

void Java_chronotext_android_cinder_CinderRenderer_setup(JNIEnv *env, jobject obj, jint width, jint height, jint accelerometerRotation)
{
	gDelegate->setup(width, height, accelerometerRotation);
}

void Java_chronotext_android_cinder_CinderRenderer_shutdown(JNIEnv *env, jobject obj)
{
    gDelegate->shutdown();
    delete gDelegate;
}

void Java_chronotext_android_cinder_CinderRenderer_event(JNIEnv *env, jobject obj, jint id)
{
	gDelegate->event(id);
}

void Java_chronotext_android_cinder_CinderRenderer_draw(JNIEnv *env, jobject obj)
{
	gDelegate->draw();
}

void Java_chronotext_android_cinder_CinderRenderer_addTouch(JNIEnv *env, jobject obj, jfloat x, jfloat y)
{
	gDelegate->addTouch(x, y);
}

void Java_chronotext_android_cinder_CinderRenderer_updateTouch(JNIEnv *env, jobject obj, jfloat x, jfloat y)
{
	gDelegate->updateTouch(x, y);
}

void Java_chronotext_android_cinder_CinderRenderer_removeTouch(JNIEnv *env, jobject obj, jfloat x, jfloat y)
{
	gDelegate->removeTouch(x, y);
}

void Java_chronotext_android_cinder_CinderRenderer_sendMessage(JNIEnv *env, jobject obj, jint what, jstring body)
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
