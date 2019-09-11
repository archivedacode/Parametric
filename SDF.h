//
//  SDF.h
//  EQ10
//
//  State variable filter, for computing the coefficients for parametric type filters.
//
//  Created by David Ross on 03/01/2019.
//  Copyright © 2019 David Ross. All rights reserved.
//

#ifndef SDF_h
#define SDF_h

#include "math.h"

typedef enum { LS, HS, PK, NA } SDFType;

#pragma once

class SDF
{
public:
    SDF(void)
    {
        reset();
        
        isOn = true;
        
        q = 0.0f;
        frequency = 0.0f;
        
        this->filterType = NA;
    }
    
    ~SDF(void) { }
    
    void setFilterType(SDFType type)
    {
        this->filterType = type;
    }
    
    void setIsOn(bool on)
    {
        this->isOn = on;
    }
    
    void reset()
    {
        gain = 0.0;
        
        v1L = v2L = v3L = ic1eqL = ic2eqL = 0.0;
        v1R = v2R = v3R = ic1eqR = ic2eqR = 0.0;
        
        m0 = m1 = m2 = 0.0;
        a1 = a2 = a3 = 0.0;
    }
    
    float FilterLeft(float xL)
    {
        v3L = xL - ic2eqL;
        v1L = a1 * ic1eqL + a2*v3L;
        v2L = ic2eqL + a2*ic1eqL + a3*v3L;
        
        ic1eqL = 2.0*v1L - ic1eqL;
        ic2eqL = 2.0*v2L - ic2eqL;
        
        return (m0*xL + m1*v1L + m2*v2L);
    }
    
    float FilterRight(float xR)
    {
        v3R = xR - ic2eqR;
        v1R = a1 * ic1eqR + a2*v3R;
        v2R = ic2eqR + a2*ic1eqR + a3*v3R;
        
        ic1eqR = 2.0*v1R - ic1eqR;
        ic2eqR = 2.0*v2R - ic2eqR;
        
        return (m0*xR + m1*v1R + m2*v2R);
    }
    
    void CalcCoeffs(float gain, float frequency, float q)
    {
        this->q = q;
        this->gain = gain;
        this->frequency = frequency;
        
        float A = powf(10.0, (gain / 40.0));
        float g = tan(M_PI * frequency / 44100.0);
        float k = 0.0;
        
        switch(this->filterType)
        {
            case LS:
                
                k = 1.0 / q;
                
                a1 = 1.0 / (1.0 + g*(g+k));
                a2 = g*a1;
                a3 = g*a2;
                
                m0 = 1.0;
                m1 = k*(A - 1.0);
                m2 = (A*A - 1.0);
                
                break;
                
            case HS:
                
                k = 1.0 / (q*A);
                
                a1 = 1.0 / (1.0 + g*(g+k));
                a2 = g*a1;
                a3 = g*a2;
                
                m0 = A*A;
                m1 = k*(1.0 - A) * A;
                m2 = (1.0 - A*A);
                
                break;
                
            case PK:
                
                k = 1.0 / (q*A);
                
                a1 = 1.0 / (1.0 + g*(g+k));
                a2 = g*a1;
                a3 = g*a2;
                
                m0 = 1.0;
                m1 = k*(A*A - 1.0);
                m2 = 0.0;
                
                break;
                
            case NA:
                reset();
                break;
                
            default:
                reset();
                break;
        }
    }
    
    float GetGain(void)
    {
        return this->gain;
    }
    
    float GetFrequency(void)
    {
        return this->frequency;
    }
    
    float GetQ(void)
    {
        return this->q;
    }
    
    bool isOn;
    
    float q;
    float gain;
    float frequency;
    
    SDFType filterType;
    
private:
    float m0, m1, m2;
    float a1, a2, a3;
    
    float v1L, v2L, v3L;
    float ic1eqL, ic2eqL;
    
    float v1R, v2R, v3R;
    float ic1eqR, ic2eqR;
};

#endif /* SDF_h */
