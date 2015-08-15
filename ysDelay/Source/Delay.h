//
//  Delay.h
//  ysDelay
//
//  Created by Ryan Foo on 8/9/15.
//
//

#ifndef __ysDelay__Delay__
#define __ysDelay__Delay__

#include <stdio.h>
// For jassert
#include "../JuceLibraryCode/JuceHeader.h"
// For memset/memory operations
#include <string.h>

#define INIT_SAMPLE_RATE        44100
#define DELAY_MAX               (2*INIT_SAMPLE_RATE)

class Delay {
    
public:
    // Constructor
    Delay();
    // Deconstructor
    ~Delay();
    
    // Parameter Data Structure
    struct Parameters {
        Parameters() noexcept
        : delay(5.f),
        fdbk(0.1f),
        mod(0.25f),
        mix(0.5f)
        {}
        
        float delay;
        float fdbk;
        float mod;
        float mix;
    };
    
    // Get Parameters
    const Parameters& getParameters() const noexcept {
        return parameters;
    };
    
    // Set Parameters
    void setParameters(const Parameters& newParams);
    
    // Set Sample Rate
    void setSampleRate(const double sampleRate);
    
    // Clear buffers
    void prepareToPlay();
    
    // Audio Processing Methods
    void processMono(float* const samples, const int numSamples);
    void processStereo(float* const left, float* const right, const int numSamples);
    
private:
    // Parameters DS
    Parameters parameters;
    // Delay Circular Buffers
    float delayL[1024], delayR[1024];
    // Delay Circular Buffer Reader/Writer
    int readerL, readerR, writerL, writerR;
    // Sample Rate
    double currentSampleRate;
    
};

#endif /* defined(__ysDelay__Delay__) */
