#include "LineDomain.h"

LineDomain::LineDomain(int size) :
    v(size+4, 0)
{
    L = size;
    L2 = L * L;
}

float &LineDomain::at(int l)
{
    return v.at(l+2);
}

void LineDomain::clear()
{
    for (int i = 0; i < v.size(); i++)
    {
        v[i] = 0;
    }
}

float LineDomain::dxx(int l)
{
    return L2 * (at(l + 1) - 2 * at(l) + at(l - 1));
}

float LineDomain::dxxxx(int l)
{
    return L2 * (at(l+2) - 4 * at(l+1) + 6 * at(l) - 4 * at(l-1) + at(l-2));
}

void LineDomain::prepareClampedBoundaryLeft()
{
    at(-2) = 0;
    at(-1) = 0;
}

void LineDomain::prepareClampedBoundaryRight()
{
    at(L) = 0;
    at(L+1) = 0;
}

void LineDomain::prepareFreeBoundaryLeft()
{
    at(-2) = 3 * at(0) - 2 * at(1);
    at(-1) = 2 * at(0) - at(1);
}

void LineDomain::prepareFreeBoundaryRight()
{
    at(L) = 2 * at(L-1) - at(L-2);
    at(L+1) = 3 * at(L-1) - 2 * at(L-2);
}

