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

float LineDomain::get(int l) const
{
    return v[l+2];
}

void LineDomain::clear()
{
    for (int i = 0; i < v.size(); i++)
    {
        v[i] = 0;
    }
}

float LineDomain::dxf(int l)
{
    return L * (get(l+1) - get(l));
}

float LineDomain::dxx(int l)
{
    return L2 * (get(l + 1) - 2 * get(l) + get(l - 1));
}

float LineDomain::dxxxx(int l)
{
    return L2 * (get(l+2) - 4 * get(l+1) + 6 * get(l) - 4 * get(l-1) + get(l-2));
}

void LineDomain::prepareClampedBoundaryLeft()
{
    at(-2) = get(0);
    at(-1) = 0;
}

void LineDomain::prepareClampedBoundaryRight()
{
    at(L) = 0;
    at(L+1) = get(L-1);
}

void LineDomain::prepareFreeBoundaryLeft()
{
    at(-2) = 3 * get(0) - 2 * get(1);
    at(-1) = 2 * get(0) - get(1);
}

void LineDomain::prepareFreeBoundaryRight()
{
    at(L) = 2 * get(L-1) - get(L-2);
    at(L+1) = 3 * get(L-1) - 2 * get(L-2);
}

