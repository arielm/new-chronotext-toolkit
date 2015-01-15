APP_PLATFORM := android-15
APP_STL      := gnustl_static
APP_CPPFLAGS := -fexceptions -frtti
APP_CPPFLAGS += -std=c++11 -D_LIBCPP_VERSION -Wno-format-security

NDK_TOOLCHAIN_VERSION := 4.8

APP_ABI := armeabi-v7a
#APP_OPTIM := debug
APP_MODULES := PathWorks1
