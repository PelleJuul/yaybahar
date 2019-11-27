#pragma once

#include <vector>

class Plate
{
    public:

    enum BoundaryCondition
    {
        Clamped,
        SimplySupported
    };

    Plate(int width, int height, float h, int sampleRate);
    void computeNextSample();
    void set(int x, int y, float v);
    float get(int x, int y);

    float wavespeed = 400;

    private:
    float h;
    float k;
    std::vector<float> u;
    std::vector<float> up;
    std::vector<float> un;
    int width;
    int height;

    BoundaryCondition boundaryY = Clamped;
    BoundaryCondition boundaryX = SimplySupported;


    float computeDxx(int x, int y);
    float computeDxxxx(int x, int y);
    float computeDyy(int x, int y);
    float computeDyyyy(int x, int y);
    float computeLaplacian(int x, int y);

    float computeDxxxxClampedLeft(int y);
    float computeDxxxxClampedRight(int y);
};