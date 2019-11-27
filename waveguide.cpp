#include "waveguide.h"
#include <vector>
#include <cmath>

Waveguide::Waveguide(int length) :
    buffer(length, 0)
{
    pos = 0;
}

void Waveguide::write(float x)
{
    buffer[pos] = x;
    
    pos += 1;

    if (pos >= buffer.size())
    {
        pos = 0;
    }
}

float Waveguide::read(float l)
{
    int i = pos - floor(l);
    int j = pos - ceil(l);
    float a = l - floor(l);

    if (i >= (int)buffer.size())
    {
        i -= buffer.size();
    }
    else if (i < 0)
    {
        i += buffer.size();
    }

    if (j >= (int)buffer.size())
    {
        j -= buffer.size();
    }
    else if (j < 0)
    {
        j += buffer.size();
    }

    float x1 = buffer[i];
    float x2 = buffer[j];
    float y = (1 - a) * x1 + a * x2;
    return y;
}

float *Waveguide::data()
{
    return buffer.data();
}

