LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Scalability

CHR_SRC = ../../../../../../../Cinder/blocks/new-chronotext-toolkit/src
UTF8_CPP_SRC = ../../../../../../../Cinder/include/utf8-cpp

LOCAL_C_INCLUDES := ../src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(CHR_SRC)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(UTF8_CPP_SRC)

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES += ../src/AndroidMain.cpp

LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/InputSource.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/cinder/CinderApp.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderDelegate.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/android/cinder/CinderSketchComplex.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Handler.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/Looper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/os/MessageQueue.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/PVRHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/Texture.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureHelper.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/texture/TextureManager.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/Utils.cpp
LOCAL_SRC_FILES += $(CHR_SRC)/chronotext/utils/glUtils.cpp

#LOCAL_CFLAGS := -ffast-math -O3
LOCAL_CFLAGS += -g

LOCAL_CFLAGS += -DCHR_COMPLEX

LOCAL_LDLIBS := -llog -landroid
LOCAL_STATIC_LIBRARIES := cinder boost_system boost_filesystem boost_thread freeimage ft2 android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,cinder)
$(call import-module,boost)
