#include "plate.h"
#include <vector>
#include <stdio.h>

#define p2(x) (x * x)

Plate::Plate(int width, int height, float h, int sampleRate) :
    u(width * height, 0), 
    up(width * height, 0), 
    un(width * height, 0) 
{
    this->width = width;
    this->height = height;
    this->h = h;
    this->k = 1.0 / sampleRate;
}

void Plate::computeNextSample()
{
    for (int y = 0; y < height; y++)
    {
        int i = y * width + 0;

        float lap = computeDxxxxClampedLeft(y) + computeDyyyy(0, y);
        un[i] = -p2(k) * p2(wavespeed) * lap + 2 * u[i] - up[i];

        i = y * width + width - 1;

        lap = computeDxxxxClampedRight(y) + computeDyyyy(width - 1, y);
        un[i] = -p2(k) * p2(wavespeed) * lap + 2 * u[i] - up[i];
    }

    for (int x = 1; x < width-1; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int i = y * width + x;
            float lap = computeDxxxx(x, y) + computeDyyyy(x, y);
            un[i] = -p2(k) * p2(wavespeed) * lap + 2 * u[i] - up[i];
        }
    }

    for (int i = 0; i < u.size(); i++)
    {
        up[i] = u[i];
        u[i] = un[i];
    }
}

float Plate::computeDxx(int x, int y)
{
    return (1.0 / h * h) * (get(x - 1, y) - 2 * get(x, y) + get(x + 1, y));
}

float Plate::computeDxxxx(int x, int y)
{
    // float v1 = (1.0 / h * h) * (computeDxx(x - 1, y) -  2 * computeDxx(x, y) + computeDxx(x + 1, y));
    // float hr4 = (1.0 / (h * h * h * h));
    float v2 = (get(x - 2 , y) - 4 * get(x - 1, y) + 6 * get(x, y) - 4 * get(x + 1, y) + get(x + 2, y));
    return v2;
}

float Plate::computeDyy(int x, int y)
{
    return (1.0 / h * h) * (get(x, y - 1) - 2 * get(x, y) + get(x, y + 1));
}

float Plate::computeDyyyy(int x, int y)
{
    return (1.0 / h * h) * (computeDyy(x, y - 1) -  2 * computeDyy(x, y) + computeDyy(x, y + 1));
}

float Plate::computeLaplacian(int x, int y)
{
    return computeDxx(x, y) + computeDyy(x, y);
}

float Plate::computeDxxxxClampedLeft(int y)
{
    float u2 = u[y * width + 2];
    float u1 = u[y * width + 1];
    float u0 = u[y * width + 0];
    return (u2 - 4 * u1 + 6 * u0);
}

float Plate::computeDxxxxClampedRight(int y)
{
    float u2 = u[y * width + width - 3];
    float u1 = u[y * width + width - 2];
    float u0 = u[y * width + width - 1];
    return (u2 - 4 * u1 + 6 * u0);
}

void Plate::set(int x, int y, float v)
{
    int i = y * width + x;
    u[i] = v;
}

float Plate::get(int x, int y)
{
    if (y < 0 || y >= height || x < 0 || x >= width)
    {
        return 0;
    }
    else
    {
        int i = y * width + x;
        return u[i];
    }
}
