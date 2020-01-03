#include "BowedString.h"
#include "fds.h"
#include <cmath>
#include <pal.h>

BowedString::BowedString(int L, float fs) :
    ua(L),
    ub(L),
    uc(L),
    f(L),
    u(ua),
    un(ub),
    up(uc)
{
    this->fs = fs;
    this->L = L;

    h = 1.0 / (float)L;
    k = 1.0 / fs;
    vrel = vb;

    c1 = 1 / (1 + k*sigma0);
    c2 = 1 / (2*k);
};

void BowedString::setTensionFromWavespeed(float wavespeed)
{
    T = wavespeed * wavespeed * mu;
}

float BowedString::getNextSample()
{
    // Compute bowing point
    int lb = round(L * 0.2);

    // Update up to bowing point

    for (int l = 0; l < lb; l++)
    {
        un.at(l) = c1 * update(l, 0, 0);
    }
 
    float delta = 1000.0;
    sqrt2a = sqrt(2 * a);

    for (int i = 0; i < 50 && ((delta < -10e-4) || (delta > 10e-4)); i++)
    {
        float num = c2 * c1 * update(lb, Fb, vrel) - c2 * up.at(lb) - vb - vrel;
        float denom = c2 * pow2(k) * Fb * thetad(a, vrel) - 1;
        delta = (num / denom);
        vrel = vrel - delta;
    }

    un.at(lb) = c1 * update(lb, Fb, vrel);

    for (int l = lb+1; l < L; l++)
    {
        un.at(l) = c1 * update(l, 0, 0);
    }

    LineDomain &uswap = un;
    up = u;
    u = un;
    un = uswap;

    f.clear();

    return u.at(round(L * 0.8));
}

void BowedString::drawGui()
{
    ImGui::Begin("Bowed String");
    ImGui::SliderFloat("wavespeed", &wavespeed, 1, 800);
    ImGui::SliderFloat("bow force", &Fb, 0, 5000);
    ImGui::SliderFloat("bow velocity", &vb, -0.5, 0.5);
    ImGui::SliderFloat("bow characteristic", &a, 0, 1000);
    // ImGui::PlotLines("String Displacement", u.data(), u.size(), 0, "", -1e-3, 1e-3, ImVec2(0,80));
    ImGui::End();
}

float BowedString::theta(float a, float eta)
{
    return sqrt2a * eta * exp(-a * pow2(eta) + 0.5);
}

float BowedString::thetad(float a, float eta)
{
    return sqrt2a * (exp(-a * pow2(eta) + 0.5) - 2 * a * pow2(eta) * exp(-a * pow2(eta) + 0.5));
}

float BowedString::update(int l, float Fb, float vrel)
{
    return (
          pow2(k) * pow2(wavespeed) * u.dxx(l)
        - pow2(k) * Fb * theta(a, vrel)
        + sigma0 * k * up.at(l)
        + 2 * u.at(l)
        - up.at(l)
        + pow2(k) * f.at(l));
}