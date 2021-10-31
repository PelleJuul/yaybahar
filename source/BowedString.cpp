#include "BowedString.h"
#include "fds.h"
#include <cmath>
#include "../pal/pal.h"

BowedString::BowedString(int L, float fs) :
    ua(L),
    ub(L),
    uc(L),
    f(L),
    dxxp(L),
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
    calculateDerivedParameters();
    ua.prepareClampedBoundaryLeft();
    ua.prepareClampedBoundaryRight();
    ub.prepareClampedBoundaryLeft();
    ub.prepareClampedBoundaryRight();
    uc.prepareClampedBoundaryLeft();
    uc.prepareClampedBoundaryRight();
};

void BowedString::calculateDerivedParameters()
{
    A = M_PI * r * r;
    alpha = (E * A) / T;
    M = p * A;
    I = 0.5 * M * r * r;
    kappa2 = (E * I) / M;
    omega2 = T / M;
    Fbm = Fb / M;
}

void BowedString::setTensionFromWavespeed(float wavespeed)
{
    T = wavespeed * wavespeed * p * A;
    calculateDerivedParameters();
}

float BowedString::getNextSample()
{
    // Compute g

    float sum = 0;

    for (int l = 0; l < u.size(); l++)
    {
        sum += h * pow2(u.dxf(l));
    }

    g = 1 + 0.5 * alpha * sum;

    // Compute bowing point
    int lb = int(L * 0.1732);

    // Update up to bowing point

    for (int l = 0; l < lb; l++)
    {
        float dxx = u.dxx(l);
        un.at(l) = update(l, 0, 0, dxx);
        dxxp.at(l) = dxx;
    }
 
    float dxxlb = u.dxx(lb);
    float delta = 1000.0;
    sqrt2a = sqrt(2 * a);
    float phi = 0;

    for (int i = 0; i < 50 && fabs(delta) > 10e-4; i++)
    {
        float expVrel = expf(-a * pow2(vrel) + 0.5);
        phi = sqrt2a * vrel * expVrel;
        float phid = sqrt2a * (expVrel - 2 * a * pow2(vrel) * expVrel);

        f.at(lb) = -Fbm * phi;
        float num = c2 * update(lb, Fbm, vrel, dxxlb) - c2 * up.get(lb) - vb - vrel;
        float denom = c2 * pow2(k) * -Fbm * phi - 1;
        delta = (num / denom);
        vrel = vrel - delta;
    }

    f.at(lb) = -Fbm * phi;
    un.at(lb) = update(lb, Fb, vrel, dxxlb);
    dxxp.at(lb) = dxxlb;

    for (int l = lb+1; l < L; l++)
    {
        float dxx = u.dxx(l);
        un.at(l) = update(l, 0, 0, dxx);
        dxxp.at(l) = dxx;
    }

    LineDomain &uswap = un;
    up = u;
    u = un;
    un = uswap;

    f.clear();

    return u.at(L * 0.8);
}

void BowedString::drawGui()
{
    ImGui::Begin("Bowed String");
    ImGui::SliderFloat("Wavespeed", &guiWavespeed, 1, 800);
    // ImGui::InputFloat("Young's modulus", &E, 1e4, 1e5, 0);
    ImGui::SliderFloat("Bowing force", &Fb, 0, 100);
    // ImGui::LabelText("Fbm", "%.2f", Fbm);
    // ImGui::SliderFloat("bow velocity", &vb, -0.5, 0.5);
    // ImGui::SliderFloat("bow characteristic", &a, 0, 100);
    // ImGui::PlotLines("String Displacement", u.data(), u.size(), 0, "", -1e-3, 1e-3, ImVec2(0,80));
    ImGui::End();

    setTensionFromWavespeed(guiWavespeed);
    calculateDerivedParameters();
}

float BowedString::theta(float a, float eta)
{
    return sqrt2a * eta * exp(-a * pow2(eta) + 0.5);
}

float BowedString::thetad(float a, float eta)
{
    return sqrt2a * (exp(-a * pow2(eta) + 0.5) - 2 * a * pow2(eta) * exp(-a * pow2(eta) + 0.5));
}

float BowedString::update(int l, float Fb, float vrel, float dxx) const
{
    return c1 * (
          pow2(k) * omega2 * g * dxx
        - pow2(k) * kappa2 * u.dxxxx(l)
        + sigma0 * k * up.get(l)
        + 2 * sigma1 * k * (dxx - dxxp.get(l))
        + 2 * u.get(l)
        - up.get(l)
        + pow2(k) * f.get(l));
}