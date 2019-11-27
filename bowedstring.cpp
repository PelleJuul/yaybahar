#include "bowedstring.h"
#include "fds.h"
#include <cmath>
#include <pal.h>

BowedString::BowedString(int L, float fs) :
    u(L, 0),
    up(L, 0),
    un(L, 0)
{
    this->fs = fs;
    this->L = L;

    h = 1.0 / (float)L;
    k = 1.0 / fs;
    vrel = vb;

    c1 = 1 / (1 + k*sigma0);
    c2 = 1 / (2*k);
};

inline float theta(float a, float eta)
{
    return sqrt(2 * a) * eta * exp(-a * pow2(eta) + 0.5);
}

inline float thetad(float a, float eta)
{
    return sqrt(2 * a) * (exp(-a * pow2(eta) + 0.5) - 2 * a * pow2(eta) * exp(-a * pow2(eta) + 0.5));
}

inline float update(
    const std::vector<float> &u,
    const std::vector<float> &up,
    int l,
    float vrel,
    float Fb,
    float k,
    float rh,
    float wavespeed,
    float a,
    float sigma0)
{
    return (
          pow2(k) * pow2(wavespeed) * dxx(rh, u, l)
        - pow2(k) * Fb * theta(a, vrel)
        + sigma0 * k * up[l]
        + 2 * u[l]
        - up[l]);
}

float BowedString::getNextSample()
{
    for (int l = 0; l < L; l++)
    {
        float Fbl = 0;

        if (l == round(L * 0.2))
        {
            Fbl = Fb;

            for (int i = 0; i < 100; i++)
            {
                float f = c2 * c1 * update(u, up, l, vrel, Fbl, k, L, wavespeed, a, sigma0) - c2 * up[l] - vb - vrel;
                float fd = c2 * pow2(k) * Fbl * thetad(a, vrel) - 1;
                float vreln = vrel - (f / fd);

                if (fabs(vreln - vrel) < 10e-4)
                {
                    vrel = vreln;
                    break;
                }

                vrel = vreln;
            }
        }

        un[l] = c1 * update(u, up, l, vrel, Fbl, k, L, wavespeed, a, sigma0);
    }

    for (int l = 0; l < L; l++)
    {
        up[l] = u[l];
        u[l] = un[l];
    }

    return u[round(L * 0.8)];
}

void BowedString::drawGui()
{
    ImGui::Begin("Bowed String");
    ImGui::SliderFloat("wavespeed", &wavespeed, 1, 400);
    ImGui::SliderFloat("bow force", &Fb, 0, 5000);
    ImGui::SliderFloat("bow velocity", &vb, -0.5, 0.5);
    ImGui::SliderFloat("bow characteristic", &a, 0, 1000);
    ImGui::PlotLines("String Displacement", u.data(), u.size(), 0, "", -1e-3, 1e-3, ImVec2(0,80));
    ImGui::End();
}