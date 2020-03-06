#pragma once

#include <cmath>
#include <vector>
#include <stdio.h>

class Scope
{
    public:
    Scope(int l);

    void write(float x);

    void draw();

    private:
    std::vector<float> data;
    float trigger = 0.0;
    float prev = 0;
    bool triggerEnabled = false;
    bool freeze = false;
};