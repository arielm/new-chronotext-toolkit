/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "portaudio.h"

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

class AudioLoopDelegate
{
public:
    virtual void renderCallback(int bufferSize, float *bufferData, double timeStamp) = 0;
};

class AudioLoopImplMsw
{
    bool initialized;

	boost::thread *thread;
	boost::condition cond;
	boost::mutex mutex;

	PaStream *stream;

	static int staticQueueCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
    {
        AudioLoopImplMsw *instance = reinterpret_cast<AudioLoopImplMsw*>(userData);
        instance->delegate->renderCallback(framesPerBuffer, (float*)outputBuffer, timeInfo->currentTime);

		return 0;
    }
    
	void run();

public:
    AudioLoopDelegate *delegate;

    AudioLoopImplMsw() : delegate(NULL), initialized(false) {}

    bool init();
    void shutdown();
    
    void start();
    void stop();
    
    void setVolume(float volume);
};
