#pragma once

#include <vector>

class RadialDomain
{
    public:
    RadialDomain(int numRadialPoints, int numAngularPoints);

    RadialDomain(const RadialDomain &other);

    void clear();

    float laplaceCenter();

    float laplaceMiddle(int l, int m);

    float laplacePerimeter(int m);

    /// Fast at
    /// Assumes that we're not getting any values from the perimeter or the
    /// center.
    float fat(int l, int m);

    float &at(int l, int m);

    float &operator()(int l, int m)
    {
        return at(l, m);
    };

    private:
    int numRadialPoints = 0;
    int numAngularPoints = 0;
    float hᵣ = 0;
    float hΘ = 0;
    float center = 0;
    float zero = 0;
    float c1 = 0;
    float c2 = 0;
    std::vector<float> data;
};