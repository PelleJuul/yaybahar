#pragma once
#include "Domain2d.h"

struct RectMembrane
{
    // Basic parameters
    float k;
    float k2;

    // Membrane parameters
    float E = 3e6;      // Young's modulus, Pa (N / m^2)
    float p = 1090;     // Density, kg / m^3 (nylon)
    float h = 0.0003;   // Thickness or height, m
    float T = 40;       // Tension, N
    float sigma0 = 1.0; // Dampening, unitless
    float sigma1 = 1e-2;// Frequency dependent dampening, unitless
    float nu = 0.41;    // Poisson's ratio, unitless

    // Derived membrane parameters
    float M;            // Mass, kg
    float D;            // Flextural coefficient
    float omega2;       // Wavespeed squared, m / s
    float alpha;        // Increse of tension due to amplitude, unitless

    // State variables
    Domain2d ua;
    Domain2d ub;
    Domain2d uc;
    Domain2d f;
    Domain2d laplacep;
    Domain2d &u;
    Domain2d &up;
    Domain2d &un;

    // Intermediate variables
    float c1 = 0;
    float g = 0;

    // guiVariable
    float guiWavespeed = 100;

    // Other variables
    bool strike = false;

    RectMembrane(int x, int y, float fs);

    void addForce(int x, int y, float force);

    void compute();

    void drawGui();

    void calculateDerivedParameters();

    float getMass() { return M; };

    void setTensionFromWavespeed(float wavespeed);

    float update(int x, int y);
};