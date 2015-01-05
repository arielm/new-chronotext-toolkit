/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/android/system/FileHelper.h"
#include "chronotext/android/cinder/JNI.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace file
    {
        namespace intern
        {
            AAssetManager *assetManager = nullptr;
            fs::path internalDataPath;
            fs::path externalDataPath;
            fs::path apkPath;
            
            // ---
            
            bool setup = false;
        }

        fs::path defaultPath()
        {
            return Helper::getAndroidInternalDataPath();
        }

        // ---
        
        void Helper::setup(const system::InitInfo &initInfo)
        {
            if (!intern::setup)
            {
                JNIEnv *env = jni::getEnv();
                
                jmethodID getAssetsMethod = env->GetMethodID(env->GetObjectClass(initInfo.androidContext), "getAssets", "()Landroid/content/res/AssetManager;");
                intern::assetManager = AAssetManager_fromJava(env, env->CallObjectMethod(initInfo.androidContext, getAssetsMethod));
                
                //
                
                jmethodID getFilesDirMethod = env->GetMethodID(env->GetObjectClass(initInfo.androidContext), "getFilesDir", "()Ljava/io/File;");
                jobject filesDirObject = env->CallObjectMethod(initInfo.androidContext, getFilesDirMethod);
                jmethodID getAbsolutePathMethod = env->GetMethodID(env->GetObjectClass(filesDirObject), "getAbsolutePath", "()Ljava/lang/String;");
                jstring internalDataPath = (jstring)env->CallObjectMethod(filesDirObject, getAbsolutePathMethod);
                
                intern::internalDataPath = fs::path(jni::toString(internalDataPath));
                
                //
                
                jclass environmentClass = env->FindClass("android/os/Environment");
                jmethodID getExternalStorageDirectoryMethod = env->GetStaticMethodID(environmentClass, "getExternalStorageDirectory",  "()Ljava/io/File;");
                jobject externalStorageDirectoryObject = env->CallStaticObjectMethod(environmentClass, getExternalStorageDirectoryMethod);
                jstring externalDataPath = (jstring)env->CallObjectMethod(externalStorageDirectoryObject, getAbsolutePathMethod);
                
                intern::externalDataPath = fs::path(jni::toString(externalDataPath));
                
                //
                
                jmethodID getPackageCodePathMethod = env->GetMethodID(env->GetObjectClass(initInfo.androidContext), "getPackageCodePath", "()Ljava/lang/String;");
                jstring apkPath = (jstring)env->CallObjectMethod(initInfo.androidContext, getPackageCodePathMethod);
                
                intern::apkPath = fs::path(jni::toString(apkPath));
                
                // ---
                
                intern::setup = true;
            }
        }
        
        void Helper::shutdown()
        {
            if (intern::setup)
            {
                intern::assetManager = nullptr;
                intern::setup = false;
            }
        }
        
        // ---
        
        AAssetManager* Helper::getAndroidAssetManager()
        {
            assert(intern::assetManager);
            return intern::assetManager;
        }
        
        fs::path Helper::getAndroidInternalDataPath()
        {
            return intern::internalDataPath;
        }
        
        fs::path Helper::getAndroidExternalDataPath()
        {
            return intern::externalDataPath;
        }
        
        fs::path Helper::getAndroidApkPath()
        {
            return intern::apkPath;
        }
    }
}
