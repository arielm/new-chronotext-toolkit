#pragma once

#include "cinder/Timer.h"
#include "chronotext/utils/Utils.h"

ci::Timer ___timer___;
double ___tmp___;

#define TIMING_START ___timer___.start();
#define TIMING_STOP(MSG) ___tmp___ = ___timer___.getSeconds(); LOGI << MSG << ": " << ___tmp___ << std::endl;
