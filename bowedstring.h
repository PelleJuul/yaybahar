#pragma once

#include <vector>
#include "LineDomain.h"

struct BowedString
{
    // Basic parameters
    float fs;

    // String parameters
    int L;
    float wavespeed = 200;
    float sigma0 = 2.0;

    // Bow parameters
    float Fb = 1000;
    float vb = 0.2;
    float a = 50;

    // Derived parameters
    float h;
    float k;
    float c1;
    float c2;

    // State variables
    LineDomain ua;
    LineDomain ub;
    LineDomain uc;
    LineDomain &u;
    LineDomain &un;
    LineDomain &up;
    float vrel;

    BowedString(int L, float fs);

    float getNextSample();
    void drawGui();
};