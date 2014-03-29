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
        float contentScale;
        float diagonal;
        
        EmulatedDevice(int width, int height, float contentScale = 0, float diagonal = 0)
        :
        size(width, height),
        contentScale(contentScale),
        diagonal(diagonal)
        {}
        
        EmulatedDevice& rotate()
        {
            this->size = this->size.yx();
            return *this;
        }
    };

    static EmulatedDevice DEVICE_IPHONE_3GS_PORTRAIT = EmulatedDevice(320, 480, 1, 3.54f);
    static EmulatedDevice DEVICE_IPHONE_4_PORTRAIT = EmulatedDevice(640, 960, 2, 3.54f);
    static EmulatedDevice DEVICE_IPHONE_5_PORTRAIT = EmulatedDevice(640, 1136, 2, 4.0f);
    
    static EmulatedDevice DEVICE_IPAD_1_PORTRAIT = EmulatedDevice(768, 1024, 1, 9.7f);
    static EmulatedDevice DEVICE_IPAD_MINI_1_PORTRAIT = EmulatedDevice(768, 1024, 1, 7.9f);

    static EmulatedDevice DEVICE_GALAXY_MINI_2_PORTRAIT = EmulatedDevice(320, 480, 1, 3.27f);
    static EmulatedDevice DEVICE_GALAXY_S_PORTRAIT = EmulatedDevice(480, 800, 1, 4.0f);
    static EmulatedDevice DEVICE_GALAXY_S3_PORTRAIT = EmulatedDevice(720, 1280, 1, 4.8f);
    static EmulatedDevice DEVICE_GALAXY_S4_PORTRAIT = EmulatedDevice(1080, 1920, 1, 5);
    
    static EmulatedDevice DEVICE_NEXUS_7_2012_PORTRAIT = EmulatedDevice(736, 1280, 1, 6.89f);
    static EmulatedDevice DEVICE_NEXUS_7_2013_PORTRAIT = EmulatedDevice(1104, 1920, 1, 6.89f);

    static EmulatedDevice DEVICE_GALAXY_TAB_10_PORTRAIT = EmulatedDevice(800, 1280, 1, 10.1f);
    static EmulatedDevice DEVICE_XOOM_1_PORTRAIT = EmulatedDevice(752, 1280, 1, 10.08f);
}

namespace chr = chronotext;
