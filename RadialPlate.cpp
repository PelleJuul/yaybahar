#include "RadialPlate.h"

#define sqr(x)   (x * x)

RadialPlate::RadialPlate(int numRadialPoints, int numAngularPoints, float sampleRate) : 
        ua(numRadialPoints, numAngularPoints),
        ub(numRadialPoints, numAngularPoints),
        uc(numRadialPoints, numAngularPoints),
        f(numRadialPoints, numAngularPoints),
        Δup(numRadialPoints, numAngularPoints),
        u(ua),
        up(ub),
        ut(uc)
{
    this->numRadialPoints = numRadialPoints;
    this->numAngularPoints = numAngularPoints;
    hr = 1.0 / numRadialPoints;
    hΘ = 1.0 / numAngularPoints;
    k = 1.0 / sampleRate;
    this->sampleRate = sampleRate;
}

void RadialPlate::calculate()
{
    // Precalculate coefficients
    float c1 = 1.0 / (1 + σ₀ * k);

    // Update non-center points not on the perimeter
    for (int l = 1; l < numRadialPoints - 1; l++)
    {
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

void RadialPlate::addForce(int l, int m, float force)
{
    f(l, m) = force;
}

float RadialPlate::get(int l, int m)
{
    return u(l, m);
}