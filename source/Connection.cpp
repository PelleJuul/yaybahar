#include "Connection.h"

Connection::Connection(float sampleRate)
{
    fs = sampleRate;
    k = 1.0f / sampleRate;
}

float Connection::calculateForce(float u1, float u2)
{
    float η = u1 - u2;

    float ω02 = ω0 * ω0;
    float ω14 = ω1 * ω1 * ω1 * ω1;
    float η2 = η * η;

    // float p = -2 / (2 * (σ / k) + ω02 + ω14 * η2);
    // float r = (2 * (σ / k) - ω02 - ω14 * η2) / (2 * (σ / k) + ω02 + ω14 * η2);

    // float ηn = p * F + r * ηp;

    // float Fn = -ω02 * 0.5 * (ηn + ηp) - ω14 * η2 * 0.5 * (ηn + ηp) - (σ / k) * (ηn - ηp);

    // float Fn = (-ω02 * 0.5 * (η + ηp) - ω14 * η2 * 0.5 * (η + ηp) - (σ / k) * (η - ηp));

    // float Fn = (-ω02 * η - ω14 * η2 * η - (σ / k) * (η - ηp));
    float Fn = -ω02 * (u1 - u2);

    // ηp = η;
    // F = Fn;

    return Fn;
}