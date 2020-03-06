#pragma once

#include <string>
#include <cmath>

class Gain
{
    public:
    float gain = 0;

    Gain(std::string name = "Gain");
    void draw();

    operator float()
    {
        return powf(10, gain / 10);
    };

    Gain &operator= (const float &value)
    {
        gain = value;
        return *this;
    };

    private:
    std::string name;
};