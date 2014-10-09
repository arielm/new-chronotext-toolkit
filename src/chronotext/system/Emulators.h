/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "EmulatedDevice.h"

namespace chronotext
{
    namespace Emulators
    {
        static EmulatedDevice IPHONE_3GS_PORTRAIT (DisplayInfo::createWithDiagonal(320, 480, 3.54f));
        static EmulatedDevice IPHONE_4_PORTRAIT (DisplayInfo::createWithDiagonal(320, 480, 3.54f, 2));
        static EmulatedDevice IPHONE_5_PORTRAIT (DisplayInfo::createWithDiagonal(320, 568, 4.00f, 2));
        
        static EmulatedDevice IPAD_1_LANDSCAPE (DisplayInfo::createWithDiagonal(1024, 768, 9.70f));
        static EmulatedDevice IPAD_MINI_1_LANDSCAPE (DisplayInfo::createWithDiagonal(1024, 768, 7.90f));
        
        static EmulatedDevice GALAXY_MINI_2_PORTRAIT (DisplayInfo::createWithDiagonal(320, 480, 3.27f));
        static EmulatedDevice GALAXY_S_PORTRAIT (DisplayInfo::createWithDiagonal(480, 800, 4.00f));
        static EmulatedDevice GALAXY_S3_PORTRAIT (DisplayInfo::createWithDiagonal(720, 1280, 4.80f));
        static EmulatedDevice GALAXY_S4_PORTRAIT (DisplayInfo::createWithDiagonal(1080, 1920, 5.00f));
        
        static EmulatedDevice GALAXY_TAB_10_LANDSCAPE (DisplayInfo::createWithDiagonal(1280, 800, 10.10f));
        static EmulatedDevice XOOM_1_LANDSCAPE (DisplayInfo::createWithDiagonal(1280, 752, 10.08f)); // XXX
        
        static EmulatedDevice NEXUS_7_2012_PORTRAIT (DisplayInfo::createWithDiagonal(800, 1280, 7.02f), WindowInfo::create(734, 1280));
        static EmulatedDevice NEXUS_7_2013_PORTRAIT (DisplayInfo::createWithDiagonal(1200, 1920, 7.02f), WindowInfo::create(1104, 1920));
        static EmulatedDevice KINDLE_FIRE_PORTRAIT (DisplayInfo::createWithDiagonal(800, 1280, 7.00f));
    }
}
