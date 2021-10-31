#include "Spring.h"
#include "../pal/pal.h"
#include "LineDomain.h"

Spring::Spring(int L, float fs) :
    ua(L),
    ub(L),
    uc(L),
    dxxp(L),
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
    if (strike)
    {
        addForce(0.2 * numNodes, 10000);
        strike = false;
    }

    u.prepareFreeBoundaryLeft();
    u.prepareFreeBoundaryRight();
    up.prepareFreeBoundaryLeft();
    up.prepareFreeBoundaryRight();

    float kappa2 = kappa * kappa;
    double c1 = (1.0 / (1.0 + k * s0));

    for (int l = 0; l < numNodes; l++)
    {
        float dxx = u.dxx(l);

        un.at(l) = c1 *
        (
            -kappa2 * k2 * u.dxxxx(l)
            + k * s0 * up.get(l)
            + 2 * k * s1 * (dxx - dxxp.get(l))
            + 2 * u.get(l)
            - up.get(l)
            + k2 * f.get(l)
        );

        dxxp.at(l) = dxx;
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
    // ImGui::SliderFloat("Independet dampening", &s0, 0, 10);

    if (ImGui::Button("Strike"))
    {
        strike = true;
    }

    // ImGui::PlotLines("String Displacement", u.data(), u.size(), 0, "", -1e-3, 1e-3, ImVec2(0,80));
};