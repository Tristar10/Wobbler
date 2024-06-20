//
//  Mu45LFO.cpp
//
//  Created by Luke Dahl on 3/4/15.


#include "Mu45LFO.h"
#include <cmath>

// constructor
Mu45LFO::Mu45LFO()
{
    for (int i = 0; i < N; i++) {
            table[i] = (double)i/N;
        }
    // init other stuff
    setFreq(1.0, 44100);
    resetPhase();
}

void Mu45LFO::updateParams(float attackParam, float speedParam, float rate, float mFs)
{
    if (speedParam == 100)
        attackParam += 60.0;

    double offset = ((attackParam - 50.0) / 62.5) < 0 ? 0 : ((attackParam - 50.0) / 62.5) * N / 2.0;
    for (int i = static_cast<int>(offset); i < N; i++) {
        if (i <= (N / 2 + (((attackParam - 50.0) / 62.5) * N / 2.0)) && i > static_cast<int>(offset)) {
            table[i] = pow((i - ((attackParam - 50.0) / 62.5) * N / 2.0) / (static_cast<double>(N) / 2), (100.0 - speedParam) / 10.0);
        } else if (i > (N / 2 + (((attackParam - 50.0) / 62.5) * N / 2.0))) {
            table[i] = 1.0;
        } else {
            table[i] = 0.0;
        }
    }
    setFreq(rate, mFs);
}


// set the frequency of the LFO (freq) and how often it will be called (fs). Both values are in Hz.
void Mu45LFO::setFreq(float freq, float fs)
{
    phase_inc = N * freq / fs;
}

// reset the phase, so that the next output is at the beginning of the wavetable
void Mu45LFO::resetPhase()
{
    phase = 0.0;
}

// update the phase of the LFO and produce an output sample
float Mu45LFO::tick()
{
    // calc output
    float outSamp, frac;
    int idx1, idx2;         // the indeces to the two table entries we will interpolate between
    
    idx1 = floor(phase);    // the first index is the integer part of phase
    idx2 = idx1 + 1;        // the second index is the next table entry
    if (idx2 >= N) {        // unless that is greater than the table length...
        idx2 = 0;           // in which case wrap around
    }
    frac = phase - idx1;    // the fraction of the index, i.e. the amount to interpolate
    
    // linearly interpolate between adjacent table entries
    outSamp  = (1-frac)*table[idx1] + frac*table[idx2];
    
    // update phase
    phase += phase_inc;
    if (phase >= N) {    // wrap around
        phase -= N;
    }
    
    return outSamp;
}













