#pragma once

#include <vector>

class LineDomain
{
    public:
    LineDomain(int size);

    float &at(int l);

    float get(int l) const;

    void clear();

    /// Compute the first order forwards difference.
    /// @param  l   The point to evaluate at.
    float dxf(int l);

    float dxx(int l);
    
    float dxxxx(int l);

    void prepareClampedBoundaryLeft();
    void prepareClampedBoundaryRight();

    void prepareFreeBoundaryLeft();
    void prepareFreeBoundaryRight();

    size_t size()
    {
        return L;
    };

    float *data()
    {
        return v.data();
    };

    private:
    float L = 0;
    float L2 = 0;
    std::vector<float> v;
};