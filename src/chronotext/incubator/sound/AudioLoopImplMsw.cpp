/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "AudioLoopImplMsw.h"

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (512)
#define OUTPUT_DEVICE Pa_GetDefaultOutputDevice()

bool AudioLoopImplMsw::init()
{
	if (!initialized)
	{
		thread = new boost::thread(&AudioLoopImplMsw::run, this);
	}
    
    return true; // XXX
}

void AudioLoopImplMsw::shutdown()
{
    if (initialized)
    {
		cond.notify_all();
		thread->join();
		
		initialized = false;
    }
}

/*
 * TODO
 */
void AudioLoopImplMsw::start()
{}

/*
 * TODO
 */
void AudioLoopImplMsw::stop()
{}

/*
 * TODO
 */
void AudioLoopImplMsw::setVolume(float volume)
{}

void AudioLoopImplMsw::run()
{
	PaError err = Pa_Initialize();

	if (err != paNoError)
	{
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
		return;
	}

	PaStreamParameters outputParameters;
	outputParameters.device = OUTPUT_DEVICE;
	outputParameters.channelCount = 1; /* MONO output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff, /* we won't output out of range samples so don't bother clipping them */
		AudioLoopImplMsw::staticQueueCallback,
		this );

	if (err != paNoError)
	{
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err) );
		return;
	}

	initialized = true;

	// ---

    err = Pa_StartStream(stream);
    if (err != paNoError) fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));

	boost::mutex::scoped_lock lock(mutex);
	cond.wait(lock);

	err = Pa_AbortStream(stream);
    if (err != paNoError) fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    
    err = Pa_CloseStream(stream);
    if (err != paNoError) fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));

	Pa_Terminate();
}
