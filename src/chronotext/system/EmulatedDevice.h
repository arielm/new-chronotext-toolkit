/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/system/SystemInfo.h"

#include "cinder/Vector.h"

namespace chronotext
{
    struct EmulatedDevice
    {
        ci::Vec2i size;
        float diagonal;
        int sizeFactor;
        
        EmulatedDevice(int width, int height, float diagonal = 0, int sizeFactor = SystemInfo::SIZE_UNDEFINED)
        :
        size(width, height),
        diagonal(diagonal),
        sizeFactor(sizeFactor)
        {}
        
        EmulatedDevice& rotate()
        {
            this->size = this->size.yx();
            return *this;
        }
    };

    static EmulatedDevice DEVICE_IPHONE_3GS_PORTRAIT = EmulatedDevice(320, 480, 3.5f, SystemInfo::SIZE_IPHONE);
    static EmulatedDevice DEVICE_IPHONE_4_PORTRAIT = EmulatedDevice(640, 960, 3.5f, SystemInfo::SIZE_IPHONE);
    static EmulatedDevice DEVICE_IPHONE_5_PORTRAIT = EmulatedDevice(640, 1136, 4, SystemInfo::SIZE_IPHONE);
    
    static EmulatedDevice DEVICE_IPAD_1_PORTRAIT = EmulatedDevice(768, 1024, 9.7f, SystemInfo::SIZE_IPAD);
    static EmulatedDevice DEVICE_IPAD_MINI_1_PORTRAIT = EmulatedDevice(768, 1024, 7.9f, SystemInfo::SIZE_IPAD_MINI);
    
    static EmulatedDevice DEVICE_GALAXY_S_PORTRAIT = EmulatedDevice(480, 800, 4, SystemInfo::SIZE_ANDROID_PHONE);
    static EmulatedDevice DEVICE_GALAXY_S3_PORTRAIT = EmulatedDevice(720, 1280, 4.8f, SystemInfo::SIZE_ANDROID_PHONE);
    static EmulatedDevice DEVICE_GALAXY_S4_PORTRAIT = EmulatedDevice(1080, 1920, 5, SystemInfo::SIZE_ANDROID_PHONE);
    
    static EmulatedDevice DEVICE_NEXUS_7_2012_PORTRAIT = EmulatedDevice(736, 1280, 6.89f, SystemInfo::SIZE_ANDROID_TABLET_MINI);
    static EmulatedDevice DEVICE_NEXUS_7_2013_PORTRAIT = EmulatedDevice(1104, 1920, 6.89f, SystemInfo::SIZE_ANDROID_TABLET_MINI);

    static EmulatedDevice DEVICE_GALAXY_TAB_10_PORTRAIT = EmulatedDevice(800, 1280, 10.1f, SystemInfo::SIZE_ANDROID_TABLET);
    static EmulatedDevice DEVICE_XOOM_1_PORTRAIT = EmulatedDevice(752, 1280, 10.1f, SystemInfo::SIZE_ANDROID_TABLET);
}

namespace chr = chronotext;
