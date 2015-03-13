LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

ifndef CINDER_PATH
    $(error CINDER_PATH MUST BE DEFINED!)
endif

###

CHR_BLOCK_PATH = $(CINDER_PATH)/blocks/new-chronotext-toolkit

CHR_USE_XFONT := 1
CHR_US_PATH := 1

include $(CHR_BLOCK_PATH)/android/Android.mk

###

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../src
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../src/*.cpp)
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)

###

LOCAL_CFLAGS += -ffast-math -O3
#LOCAL_CFLAGS += -g -DDEBUG -DFORCE_LOG

###

LOCAL_LDLIBS := -llog -landroid
LOCAL_STATIC_LIBRARIES := cinder android_native_app_glue boost_system boost_filesystem boost_thread

LOCAL_MODULE := PathWorks1
include $(BUILD_SHARED_LIBRARY)

###

$(call import-module, android/native_app_glue)

$(call import-add-path, $(CINDER_PATH)/android/prebuilt)
$(call import-module, cinder)
$(call import-module, boost)
