#pragma once

#ifndef fft_hpp
#define fft_hpp

#include <stdio.h>


class fft {
public:
    
    fft();
    ~fft();
    
    /* Calculate the power spectrum */
    void powerSpectrum(int start, int half, float *data, int windowSize,float *magnitude,float *phase, float *power, float *avg_power);
    /* ... the inverse */
    void inversePowerSpectrum(int start, int half, int windowSize, float *finalOut,float *magnitude,float *phase);
};
#endif /* fft_hpp */
