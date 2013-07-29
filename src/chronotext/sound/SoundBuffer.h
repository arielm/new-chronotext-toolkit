#pragma once

#include "cinder/DataSource.h"

/*
 * LOADING WAV FILES:
 * - PCM FORMAT
 * - MONO
 * - 16-BITS-PER-SAMPLE
 * - SAMPLE-RATE OF 44100
 *
 * WARNING: Adobe SoundBooth CAN CREATE ILL-FORMED WAV FILES
 */

class SoundBuffer
{
    ci::Buffer buffer;
    uint64_t sampleRate;
    uint64_t sampleCount;
    
public:
    void load(ci::DataSourceRef source);
    double getDuration();
    int16_t* getSamples();
    uint64_t getSampleRate();
    uint64_t getSampleCount();
};
