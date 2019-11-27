#pragma once

#include <vector>
#include "LineDomain.h"

struct Spring
{
    // Basic parameters
    float k;
    int numNodes;

    // Spring parameters
    float kappa = 5.06;
    float s0 = 1.0;
    float s1 = 10e-8;

    // Derived parameters
    float k2;

    // State variables
    LineDomain ua;
    LineDomain ub;
    LineDomain uc;
    LineDomain f;
    LineDomain &u;
    LineDomain &up;
    LineDomain &un;

    Spring(int L, float fs);

    void addForce(int l, float input)
    {
        f.at(l) += input;
    }

    float &at(int l) { return u.at(l); };

    float computeNextSample(float input);
    void drawGui();
};