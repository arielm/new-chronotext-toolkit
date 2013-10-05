/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Timer.h"
#include "chronotext/utils/Utils.h"

ci::Timer ___timer___;
double ___tmp___;

#define TIMING_START ___timer___.start();
#define TIMING_STOP(MSG) ___tmp___ = ___timer___.getSeconds(); LOGI << MSG << ": " << std::setprecision(15) << ___tmp___ << std::endl;
