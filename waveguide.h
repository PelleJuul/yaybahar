#pragma once

#include <vector>

class Waveguide
{
    public:
    Waveguide(int length);

    // write a value to the beginning of the Waveguide.
    void write(float x);

    // Read a value at the specified position in samples
    float read(float l);

    float *data();

    int size() { return buffer.size(); }

    private:
    std::vector<float> buffer;
    int pos;
};