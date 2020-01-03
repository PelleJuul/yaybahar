#pragma once

#include <vector>

class Domain2d
{
    public:
    Domain2d(int cols, int rows);

    void clear();

    void prepareClampedBoundaryTop();
    void prepareClampedBoundaryBottom();
    void prepareClampedBoundaryLeft();
    void prepareClampedBoundaryRight();

    float dxx(int x, int y);
    
    float dyy(int x, int y);

    float laplace(int x, int y);

    float &at(int x, int y);

    float &operator()(int x, int y)
    {
        return at(x, y);
    };

    private:
    int cols = 0;
    int rows = 0;
    int cols2 = 0.0;
    int rows2 = 0.0;
    float hr = 0;
    float hc = 0;
    float hr2 = 0;
    float hc2 = 0;
    std::vector<float> data;
};