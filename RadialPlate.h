#pragma once

#include "RadialDomain.h"

class RadialPlate
{
    public:
    /// Create a new radial plate model.
    /// @param  numRadialPoints     Number of radial points (from the center and
    ///                             outwards, N_r).
    /// @param  numAngularPoints    Number of angular points (going around the
    ///                             circle)
    /// @param  sampleRate          The sample rate.
    RadialPlate(int numRadialPoints, int numAngularPoints, float sampleRate);

    /// Calculate the next timestep of the plate deformation. Use the `get`
    /// function to get a value somewhere on the plate.
    void calculate();

    /// Add an external force to the plate at this instant
    /// @param  l       The radial coordinate.
    /// @param  m       The angular coordinate.
    /// @param  force   The force to apply.
    void addForce(int l, int m, float force);

    /// Get the deformation of a point on the plane.
    /// @param  l   The radial coordinate.
    /// @param  m   The angular coordinate.
    float get(int l, int m);

    private:
    float sampleRate;       // The sample rate.
    float k;                // 1 / sampleRate.
    float hr;               // 1 / numRadialPoints.
    float hΘ;               // 1 / numAngularPoints.
    float σ₀ = 0.6;     // Freq. independent dampening.
    float σ₁ = 1e-2;    // Freq. dependent dampening.
    int numRadialPoints;
    int numAngularPoints;

    RadialDomain ua;
    RadialDomain ub;
    RadialDomain uc;
    RadialDomain f;
    RadialDomain Δup;
    RadialDomain &u;
    RadialDomain &up;
    RadialDomain &ut;
    float waveSpeed = 200;  // The wave speed.
};