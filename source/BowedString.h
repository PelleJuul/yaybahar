#pragma once

#include <vector>
#include "LineDomain.h"

struct BowedString
{
    // Basic parameters
    float fs;
    int L;

    // Derived parameters
    float h;
    float k;
    float c1;
    float c2;
    float g;    // Increased tension due to amplitude

    // String parameters
    float E = 200e6;    // Young's modulus, Pa (N / m^2)
    float p = 8000;     // Density, kg / m^3 (steel)
    float r = 0.001;    // Radius, m
    float T = 40;       // Tension, N
    float sigma0 = 0.8; // Dampening, unitless
    float sigma1 = 1e-4;// Frequency dependent dampening, unitless

    // String derived parameters
    float A;            // Cross-sectional area, m^2
    float alpha;        // Increse of tension due to amplitude, unitless
    float I;            // Moment of inertia, kg m^2
    float kappa2;       // Stiffness squared, N
    float M;            // Mass, kg
    float omega2;       // Wavespeed squared, m / s

    // Bow parameters
    float Fb = 0.0;     // Bow force, N
    float vb = 0.2;     // Bow speed, m/s
    float a = 16;       // Bow characteristic, unitless

    // Bow derived parameters
    float Fbm = 0;      // Bow force scaled by mass, m / s^2

    // State variables
    LineDomain ua;
    LineDomain ub;
    LineDomain uc;
    LineDomain f;
    LineDomain dxxp;
    LineDomain &u;
    LineDomain &un;
    LineDomain &up;
    float vrel;

    // GUI Variables
    float guiWavespeed = 100;

    BowedString(int L, float fs);

    void addForce(int l, float input)
    {
        f.at(l) += input / getMass();
    }

    void calculateDerivedParameters();

    float getMass() { return M; };

    void setTensionFromWavespeed(float wavespeed);

    float getNextSample();
    float &at(int l) { return u.at(l); };
    inline float get(int l) const { return u.get(l); };
    void drawGui();

    /// Compute the bow friction given the input parameters.
    /// Make sure to precompute sqrt2a.
    /// @param  a       Bow characteristic.
    /// @param  vrel    Bow velocity relative to the stirng.
    float theta(float a, float vrel);

    /// Compute first derivative of the the bow friction given the input
    /// Make sure to precompute sqrt2a.
    // parameters.
    /// @param  a       Bow characteristic.
    /// @param  vrel    Bow velocity relative to the stirng.
    float thetad(float a, float eta);

    /// Compute the next value of point l.
    /// @param  l       The index of the point to compute for.
    /// @param  Fb      The bowing force for point l.
    /// @param  vrel    Relative bow velocity. Doesn't matter if `Fb` is zero.
    float update(int l, float Fb, float vrel, float dxx) const;

    private:
    /// precomputed values for optimizations
    float sqrt2a = 0; // sqrt(2 * a)
};