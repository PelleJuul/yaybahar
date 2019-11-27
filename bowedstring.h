#pragma once

#include <vector>

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
    float a = 100;

    // Derived parameters
    float h;
    float k;
    float c1;
    float c2;

    // State variables
    std::vector<float> u;
    std::vector<float> up;
    std::vector<float> un;
    float vrel;

    BowedString(int L, float fs);

    float getNextSample();
    void drawGui();
};