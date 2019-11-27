#include "RadialDomain.h"

#define sqr(x)   (x * x)

RadialDomain::RadialDomain(int numRadialPoints, int numAngularPoints) :
        data(numRadialPoints - 1 + numAngularPoints * (numRadialPoints - 1), 0)
{
    this->numRadialPoints = numRadialPoints;
    this->numAngularPoints = numAngularPoints;
    this->hᵣ = 1.0 / numRadialPoints;
    this->hΘ = 1.0 / numAngularPoints;
    clear();
}

RadialDomain::RadialDomain(const RadialDomain &other)
{
    center = other.center;
    numRadialPoints = other.numRadialPoints;
    numAngularPoints = other.numAngularPoints;
    data = other.data;
}

void RadialDomain::clear()
{
    center = 0;

    for (int i = 0; i < data.size(); i++)
    {
        data[i] = 0;
    }
}

float RadialDomain::laplaceCenter()
{
    float Δou = 0;

    for (int m = 0; m < numAngularPoints; m++)
    {
        // We are only interested in the first ring, so we can access the
        // data array directly.
        Δou += data[m] - center;
    }

    Δou = (4.0 / (numAngularPoints * sqr(hᵣ))) * Δou;
    return Δou;
}

float RadialDomain::laplaceMiddle(int l, int m)
{
    float Δu =
        (1.0 / (l * sqr(hᵣ))) * (
            (l + 0.5) * fat(l+1, m) - 2 * l * fat(l, m) + (l - 0.5) * at(l - 1, m))
        + (1.0 / (sqr(l) + sqr(hᵣ) + sqr(hΘ))) * (
            fat(l, m + 1) - 2 * fat(l, m) + fat(l, m - 1));

    return Δu;
}

float RadialDomain::laplacePerimeter(int m)
{
    int l = numRadialPoints - 1;

    float Δu =
        (1.0 / (l * sqr(hᵣ))) * (
            - 2 * l * fat(l, m) + (l - 0.5) * fat(l - 1, m))
        + (1.0 / (sqr(l) + sqr(hᵣ) + sqr(hΘ))) * (
            fat(l, m + 1) - 2 * fat(l, m) + fat(l, m - 1));

    return Δu;
}

float RadialDomain::fat(int l, int m)
{
    return data[(l - 1) * numAngularPoints + (m % numAngularPoints)];
}

float &RadialDomain::at(int l, int m)
{
    if (l == 0)
    {
        return center;
    }

    if (l >= numRadialPoints)
    {
        zero = 0;
        return zero;
    }

    return data[(l - 1) * numAngularPoints + (m % numAngularPoints)];
}