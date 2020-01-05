#include "Domain2d.h"    
    
Domain2d::Domain2d(int cols, int rows) :
    data((rows + 2) * (cols + 2))
{
    this->cols = cols;
    this->rows = rows;
    hc = 1.0 / cols;
    hr = 1.0 / rows;
    cols2 = cols * cols;
    rows2 = rows * rows;
    hc2 = hc * hc;
    hr2 = hr * hr;
}

void Domain2d::clear()
{
    for (int i = 0; i < data.size(); i++)
    {
        data[i] = 0;
    }
}

void Domain2d::prepareClampedBoundaryTop()
{
    for (int x = 0; x < cols; x++)
    {
        at(x, -2) = 0;
        at(x, -1) = 0;
    }
}
void Domain2d::prepareClampedBoundaryBottom()
{
    for (int x = 0; x < cols; x++)
    {
        at(x, rows) = 0;
        at(x, rows + 1) = 0;
    }
}
void Domain2d::prepareClampedBoundaryLeft()
{
    for (int y = 0; y < rows; y++)
    {
        at(-2, y) = 0;
        at(-1, y) = 0;
    }
}
void Domain2d::prepareClampedBoundaryRight()
{
    for (int y = 0; y < rows; y++)
    {
        at(cols, y) = 0;
        at(cols + 1, y) = 0;
    }
}

float Domain2d::gradientf(int x, int y)
{
    return dxf(x, y) + dyf(x, y);
}

float Domain2d::dxf(int x, int y)
{
    return cols * (at(x, y) - at(x + 1, y));
}

float Domain2d::dxx(int x, int y)
{
    return cols2 * (at(x - 1, y) - 2 * at(x, y) + at(x + 1, y));
}

float Domain2d::dyf(int x, int y)
{
    return rows * (at(x, y) - at(x, y + 1));
}
    
float Domain2d::dyy(int x, int y)
{
    return rows2 * (at(x, y - 1) - 2 * at(x, y) + at(x, y + 1));
}

float Domain2d::laplace(int x, int y)
{
    return dxx(x, y) + dyy(x, y);
}

/// Fast at
/// Assumes that we're not getting any values from the perimeter or the
/// center.
float &Domain2d::at(int x, int y)
{
    return data[cols * (y + 2) + (x + 2)];
}