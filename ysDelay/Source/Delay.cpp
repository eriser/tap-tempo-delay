//
//  Delay.cpp
//  ysDelay
//
//  Created by Ryan Foo on 8/9/15.
//
//

#include "Delay.h"


// Constructor for delay module
Delay::Delay() : currentSampleRate(INIT_SAMPLE_RATE) {
    
    // Set Initial Parameters
    setParameters(Parameters());
    // Set Initial Sample Rate
    setSampleRate(INIT_SAMPLE_RATE);
    
    // Init the readers and writers
    readerL = readerR = writerL = writerR = 0;
    // Init delay lines
    memset(delayL, 0, sizeof(int));
    memset(delayR, 0, sizeof(int));
}

// Deconstructor for delay module
Delay::~Delay() {
    
}

// Set our new parameter settings
void Delay::setParameters(const Parameters& newParams)
{
    parameters = newParams;
}

// Set new sample rate
void Delay::setSampleRate(const double sampleRate)
{
    jassert(sampleRate > 0);
    currentSampleRate = sampleRate;
}

// Mono channel processing
void Delay::processMono(float* const samples, const int numSamples)
{
    jassert (samples != nullptr);
    
    // Loop through all samples
    for (int i = 0; i < numSamples; i++)
    {
        float y = samples[i];
        // Write sample from input into delay buffer
        delayL[writerL++] = y;
        // Mix delay and dry signals
        samples[i] = y * (1.0 - parameters.mix) + 0.7 * delayL[readerL++];
        
        // Reset buffer counters when it reaches the DELAY_MAX paramter
        readerL %= DELAY_MAX;
        writerL %= DELAY_MAX;
    }
}

// Stereo channel processing
void Delay::processStereo(float* const left, float* const right, const int numSamples)
{
    jassert (left != nullptr && right != nullptr);
        
    for (int i = 0; i < numSamples; i++)
    {
        float yL = left[i];
        float yR = right[i];
        // Write sample from input into delay buffer
        delayL[writerL++] = yL;
        delayR[writerR++] = yR;
        // Mix delay and dry signals
        left[i] = yL * (1.0 - parameters.mix) + 0.7 * delayL[readerL++];
        right[i] = yR * (1.0 - parameters.mix) + 0.7 * delayR[readerR++];
        
        // Reset buffer counters when it reaches the DELAY_MAX paramter
        readerL %= DELAY_MAX;
        writerL %= DELAY_MAX;
        readerR %= DELAY_MAX;
        writerR %= DELAY_MAX;
    }
}