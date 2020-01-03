#include "fds.h"
#include <vector>

float dxx(float rh, const std::vector<float> &u, int l)
{
    if (l == u.size() - 1)
    {
        return pow2(rh) * (u[l-1] - 2 * u[l]);
    }
    else if (l == 0)
    {
        return pow2(rh) * (-2 * u[l] + u[l+1]);
    }
    else
    {
        return pow2(rh) * (u[l-1] - 2 * u[l] + u[l+1]);
    }
}