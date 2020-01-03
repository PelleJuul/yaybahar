#pragma once

#include <vector>

class LineDomain
{
    public:
    LineDomain(int size);

    float &at(int l);

    void clear();

    float dxx(int l);
    
    float dxxxx(int l);

    void prepareClampedBoundaryLeft();
    void prepareClampedBoundaryRight();

    void prepareFreeBoundaryLeft();
    void prepareFreeBoundaryRight();

    size_t size()
    {
        return v.size();
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