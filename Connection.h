#pragma once

class Connection
{
    public:
    float ω0 = 10000;
    float ω1 = 10;
    float σ = 0.3;
    float ηp = 0;
    float F = 0;
    float k = 0;
    float fs = 0;

    Connection(float sampleRate);
    float calculateForce(float u1, float u2);
};