#pragma once

// Pre-template:    65%

#include "RadialDomain.h"
#include "../pal/pal.h"

#define sqr(x)   (x * x)

template<int numRadialPoints, int numAngularPoints>
class RadialPlate
{
    public:
    /// Create a new radial plate model.
    /// @param  numRadialPoints     Number of radial points (from the center and
    ///                             outwards, N_r).
    /// @param  numAngularPoints    Number of angular points (going around the
    ///                             circle)
    /// @param  sampleRate          The sample rate.
    RadialPlate(float sampleRate) : 
        ua(numRadialPoints, numAngularPoints),
        ub(numRadialPoints, numAngularPoints),
        uc(numRadialPoints, numAngularPoints),
        f(numRadialPoints, numAngularPoints),
        Δup(numRadialPoints, numAngularPoints),
        u(ua),
        up(ub),
        ut(uc)
    {
        hr = 1.0 / numRadialPoints;
        hΘ = 1.0 / numAngularPoints;
        k = 1.0 / sampleRate;
        this->sampleRate = sampleRate;
    }

    /// Calculate the next timestep of the plate deformation. Use the `get`
    /// function to get a value somewhere on the plate.
    void calculate()
    {
        if (strike)
        {
            strike = false;
            addForce(2, 0, 5000);
        }

        // Precalculate coefficients
        float c1 = 1.0 / (1 + σ₀ * k);

        // Update non-center points not on the perimeter
        #pragma clang loop unroll(full)
        for (int l = 1; l < numRadialPoints - 1; l++)
        {
            #pragma clang loop unroll(full)
            for (int m = 0; m < numAngularPoints; m++)
            {
                // printf("%i, %i\n", l, m);
                float Δu = u.laplaceMiddle(l, m);

                float dtΔu = Δu - Δup(l, m);
                Δup(l, m) = Δu;

                ut(l, m) = c1 * (
                    sqr(k) * sqr(waveSpeed) * Δu
                    + σ₀ * k * up(l, m)
                    + 2 * σ₁ * k * dtΔu
                    + 2 * u(l, m)
                    - up(l, m)
                    + sqr(k) * f(l, m));
            }
        }

        {
            // Update non-center points on the perimeter
            int l = numRadialPoints - 1;

            #pragma clang loop unroll(full)
            for (int m = 0; m < numAngularPoints; m++)
            {
                float Δu = u.laplacePerimeter(m);

                float dtΔu = Δu - Δup(l, m);
                Δup(l, m) = Δu;

                ut(l, m) = c1 * (
                    sqr(k) * sqr(waveSpeed) * Δu
                    + σ₀ * k * up(l, m)
                    + 2 * σ₁ * k * dtΔu
                    + 2 * u(l, m)
                    - up(l, m)
                    + sqr(k) * f(l, m));
            }
        }
        
        // Update center point
        {
            float Δou = u.laplaceCenter();
            float dtΔou = Δou - Δup(0, 0);
            Δup(0, 0) = Δou;

            int l = 0;
            int m = 0;

            ut(0, 0) = c1 * (
                sqr(k) * sqr(waveSpeed) * Δou
                + σ₀ * k * up(l, m)
                + 2 * σ₁ * k * dtΔou
                + 2 * u(l, m)
                - up(l, m)
                + sqr(k) * f(l, m));
        }

        // Finalize transactions.
        RadialDomain &swap = up;
        up = u;
        u = ut;
        ut = swap;

        f.clear();
    }

    /// Add an external force to the plate at this instant
    /// @param  l       The radial coordinate.
    /// @param  m       The angular coordinate.
    /// @param  force   The force to apply.
    void addForce(int l, int m, float force)
    {
        f(l, m) = force;
    }

    /// Get the deformation of a point on the plane.
    /// @param  l   The radial coordinate.
    /// @param  m   The angular coordinate.
    float get(int l, int m)
    {
        return u(l, m);
    }

    void draw()
    {
        if (ImGui::Button("Strike"))
        {
            strike = true;
        }
    }

    void setWavespeed(float value)
    {
        waveSpeed = value;
    }

    private:
    float sampleRate;       // The sample rate.
    float k;                // 1 / sampleRate.
    float hr;               // 1 / numRadialPoints.
    float hΘ;               // 1 / numAngularPoints.
    float σ₀ = 2.0;     // Freq. independent dampening.
    float σ₁ = 1e-8;    // Freq. dependent dampening.
    bool strike = false;

    RadialDomain ua;
    RadialDomain ub;
    RadialDomain uc;
    RadialDomain f;
    RadialDomain Δup;
    RadialDomain &u;
    RadialDomain &up;
    RadialDomain &ut;
    float waveSpeed = 300;  // The wave speed.
};