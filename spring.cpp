#include "spring.h"
#include <pal.h>
#include "LineDomain.h"

Spring::Spring(int L, float fs) :
    ua(L),
    ub(L),
    uc(L),
    f(L),
    u(ua),
    up(ub),
    un(uc)
{
    k = 1 / fs;
    k2 = k * k;
    numNodes = L;
};

float Spring::computeNextSample(float input)
{
    u.prepareFreeBoundaryLeft();
    u.prepareClampedBoundaryRight();
    up.prepareFreeBoundaryLeft();
    up.prepareClampedBoundaryRight();

    float kappa2 = kappa * kappa;

    f.at(10) = 100000 * input;

    // Compute center nodes
    for (int l = 1; l < numNodes; l++)
    {
        un.at(l) = (1.0 / (1.0 + k * s0)) *
        (
            -kappa2 * k2 * u.dxxxx(l)
            + k * s0 * up.at(l)
            + 2 * k * s1 * (u.dxx(l) - up.dxx(l))
            - up.at(l)
            + 2 * u.at(l)
            - k2 * f.at(l)
        );
    }

    f.clear();

    LineDomain &uswap = up;
    up = u;
    u = un;
    un = uswap;

    return u.at(numNodes - 1);
};

void Spring::drawGui()
{
    ImGui::SliderFloat("Stiffness", &kappa, 0, 50.0);
    ImGui::SliderFloat("Independet dampening", &s0, 0, 10);
    ImGui::SliderFloat("Dependent dampening", &s1, 0, 10e-3);
    // ImGui::PlotLines("String Displacement", u.data(), u.size(), 0, "", -1e-3, 1e-3, ImVec2(0,80));
};