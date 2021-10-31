#pragma once

#include <vector>
#include "LineDomain.h"

struct Spring
{
    // Basic parameters
    float k;
    int numNodes;

    // Spring parameters
    float kappa = 5.06;     // Stiffness, unitless
    float s0 = 1.0;         // Dampening, unitless
    float s1 = 10e-6;       // Frequency dependent dampening, unitless
    float M = 0.1;          // Mass, kg  

    // Derived parameters
    float k2;

    // State variables
    LineDomain ua;
    LineDomain ub;
    LineDomain uc;
    LineDomain f;
    LineDomain dxxp;
    LineDomain &u;
    LineDomain &up;
    LineDomain &un;

    bool strike = false;

    Spring(int L, float fs);

    float &at(int l) { return u.at(l); };
    inline float get(int l) const{ return u.get(l); };
    
    void addForce(int l, float input)
    {
        f.at(l) += input / M;
    }

    float computeNextSample(float input);

    void drawGui();

    float getMass() { return M; };

    int size()
    {
        return numNodes;
    }
};