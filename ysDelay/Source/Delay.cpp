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

float Delay::interpolate(float x0, float x1, float y1, float y2, float fracDelay)
{
    // Calculate denom
    float denom = x1-x0;
    // if denom == 0, crossfade complete
    if (denom == 0) return y1;
    // calcluate slope
    float dx = (fracDelay-x0)/denom;
    // return weight sum through interpolation
    return (dx*y2 + (1-dx)*y1);
}


// Mono channel processing
void Delay::processMono(float* const samples, const int numSamples)
{
    jassert (samples != nullptr);
    
    float delay = parameters.delay*currentSampleRate;
    int rInt = (int)delay;
    float fracDelay = delay - rInt;
    
    // Loop through all samples
    for (int i = 0; i < numSamples; i++)
    {
        readerL = writerL - rInt;
        if (readerL < 0) readerL += DELAY_MAX;
        
        float yn = samples[i];
        if (writerL != readerL && parameters.delay >= 1.f) {
            readerL = writerL - rInt;
            if (readerL < 0) readerL += DELAY_MAX;
            
            int readerL_1 = readerL-1;
            if (readerL_1 < 0) readerL_1 += DELAY_MAX;
            yn = interpolate(0, 1, delayL[readerL], delayL[readerL_1], fracDelay);
            if (++readerL >= DELAY_MAX) readerL = 0;
            
            delayL[writerL++] = samples[i] + parameters.fdbk;
            if (writerL >= DELAY_MAX) writerL = 0;
        }
        
        samples[i] = samples[i] * (1-parameters.mix) + yn * parameters.mix;
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