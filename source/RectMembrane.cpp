#include "RectMembrane.h"
#include "Domain2d.h"
#include "../pal/pal.h"

RectMembrane::RectMembrane(int x, int y, float fs) :
    ua(x, y),
    ub(x, y),
    uc(x, y),
    f(x, y),
    laplacep(x, y),
    u(ua),
    up(ub),
    un(uc)
{
    k = 1.0 / fs;
    k2 = k * k;
    c1 = 1.0 / (1.0 + sigma0 * k);
    calculateDerivedParameters();
    setTensionFromWavespeed(200);

    ua.prepareClampedBoundaryTop();
    ua.prepareClampedBoundaryBottom();
    ua.prepareClampedBoundaryLeft();
    ua.prepareClampedBoundaryRight();

    ub.prepareClampedBoundaryTop();
    ub.prepareClampedBoundaryBottom();
    ub.prepareClampedBoundaryLeft();
    ub.prepareClampedBoundaryRight();

    uc.prepareClampedBoundaryTop();
    uc.prepareClampedBoundaryBottom();
    uc.prepareClampedBoundaryLeft();
    uc.prepareClampedBoundaryRight();
}

void RectMembrane::addForce(int x, int y, float force)
{
    f.at(x, y) = force / getMass();
}

void RectMembrane::compute()
{
    if (strike)
    {
        addForce(0.231 * u.getNumCols(), 0.231 * u.getNumRows(), 10000);
        strike = false;
    }

    float sum = 0;

    for (int x = 0; x < u.getNumCols(); x++)
    {
        for (int y = 0; y < u.getNumRows(); y++)
        {
            sum += u.hr * u.hc * u.gradientf(x, y);
        }
    }

    g = 1 + 0.5 * alpha * sum;

    c1 = 1.0 / (1.0 + sigma0 * k);

    for (int x = 0; x < u.getNumCols(); x++)
    {
        for (int y = 0; y < u.getNumRows(); y++)
        {
            un.at(x, y) = update(x, y);
        }
    }

    f.clear();

    Domain2d &uswap = up;
    up = u;
    u = un;
    un = uswap;
}

void RectMembrane::drawGui()
{
    ImGui::SliderFloat("wavespeed", &guiWavespeed, 1, 800);

    if (ImGui::Button("Strike"))
    {
        strike = true;
    }

    setTensionFromWavespeed(guiWavespeed);
    calculateDerivedParameters();
}

void RectMembrane::calculateDerivedParameters()
{
    float A = u.getNumCols() / u.getNumRows();
    M = A * h * p;
    D = E * h * h * h / (12 * (1 - nu * nu));
    omega2 = T / M;
    alpha = ((6 * D) / (T * A * h * h));
}

void RectMembrane::setTensionFromWavespeed(float wavespeed)
{
    calculateDerivedParameters();
    T = wavespeed * wavespeed * M;
    calculateDerivedParameters();
}

float RectMembrane::update(int x, int y)
{
    return c1 *
    (
        k2 * omega2 * g * u.laplace(x, y)
      + sigma0 * k * up.at(x, y)
      + 2 * sigma1 * k * (u.laplace(x, y) - up.laplace(x, y))
      + 2 * u.at(x, y)
      - up.at(x, y)
      + k2 * f.at(x, y)
    );
}