#include <stdio.h>
#include <cmath>
#include <vector>
#include "../pal/pal.h"
#include "BowedString.h"
#include "Spring.h"
#include "Connection.h"
#include "RadialPlate.h"
#include "RectMembrane.h"

// pal build Yaybahar.cpp BowedString.cpp Connection.cpp LineDomain.cpp RadialDomain.cpp Spring.cpp Domain2d.cpp RectMembrane.cpp -O3 -o yaybahar

int main(int argc, char **argv)
{
    float fs = 44100;
    float amp = 0;
    float stringAmp = 0.0;
    float springAmp1 = 0.0;
    float springAmp2 = 0.0;
    Gain drum1Gain("Drum 1");
    Gain plate2Gain("Plate 2");

    BowedString bowedString(55, fs);
    Spring spring1(150, fs);
    Spring spring2(150, fs);
    RectMembrane drum1(9, 9, fs);
    RectMembrane drum2(9, 9, fs);

    Connection connectionStringSpring1(44100);
    Connection connectionStringSpring2(44100);
    Connection connectionSpring1Drum1(44100);
    Connection connectionSpring2Drum2(44100);

    spring1.kappa = 15;
    spring2.kappa = 15;
    spring1.s0 = 0.5;
    spring2.s0 = 0.5;

    drum1.sigma0 = 0.5f;
    drum2.sigma0 = 0.5f;
    drum1.guiWavespeed = 200.0f;
    drum2.guiWavespeed = 100.0f;

    float ab;
    float as1;
    float as2;
    float a;

    float m1 = 1;
    float m2 = 1;

    bowedString.Fb = 0;

    std::vector<float> scope(1000, 0);
    bool impulse = false;

    Realtime rt = quickAudio([&](int n, int nc, float *in, float *out)
    {
        for (int i = 0; i < n; i++)
        {
            // Compute all forces
            float forceStringSpring1 = connectionStringSpring1.calculateForce(
                bowedString.get(3), spring1.get(0));

            float forceStringSpring2 = connectionStringSpring2.calculateForce(
                bowedString.get(3), spring2.get(0));

            float forceSpring1Drum1 = connectionSpring1Drum1.calculateForce(
                spring1.get(spring1.size() - 1),
                drum1.u.get(4, 4)
            );

            float forceSpring2Drum2 = connectionSpring2Drum2.calculateForce(
                spring2.get(spring2.size() - 1),
                drum2.u.get(4, 4)
            );

            // Apply all forces

            bowedString.addForce(3, forceStringSpring1);
            bowedString.addForce(3, forceStringSpring2);

            spring1.addForce(0, -forceStringSpring1);
            spring1.addForce(spring1.size() - 1, forceSpring1Drum1);

            spring2.addForce(0, -forceStringSpring2);
            spring2.addForce(spring2.size() - 1, forceSpring2Drum2);

            drum1.addForce(4, 4, -forceSpring1Drum1);
            drum2.addForce(4, 4, -forceSpring2Drum2);

            // Update all states

            float b = bowedString.getNextSample();
            float s1 = spring1.computeNextSample(0);
            float s2 = spring2.computeNextSample(0);

            drum1.compute();
            float d1 = drum1Gain * drum1.u.get(3, 4);

            drum2.compute();
            float d2 = plate2Gain * drum2.u.get(3, 4);

            // Write output

            float y[2];
            float p = 0.8;
            float c = ab * b;
            float l = (as1 * s1 + d1);
            float r = (as2 * s2 + d2);

            y[0] = c + p * l + (1 - p) * r;
            y[1] = c + (1 - p) * l + p * r;
            
            for (int c = 0; c < nc; c++)
            {
                // Write it to the output array here.
                out[i * nc + c] = a * 1e3 * y[c % 2];
            }
        }
    });

    quickGui([&]()
    {
        bowedString.drawGui();

        ImGui::Begin("Spring 1");
        spring1.drawGui();
        ImGui::End();

        ImGui::Begin("Spring 2");
        spring2.drawGui();
        ImGui::End();

        // ImGui::Begin("Mixer");
        // ImGui::LabelText("CPU load", "%.2f", rt.getCpuLoad());
        // ImGui::SliderFloat("String", &stringAmp, -30, 3);
        // ImGui::SliderFloat("Spring 1", &springAmp1, -30, 3);
        // ImGui::SliderFloat("Spring 2", &springAmp2, -30, 3);
        // drum1Gain.draw();
        // plate2Gain.draw();
        // ImGui::SliderFloat("Master", &amp, -30, 3);
        // // ImGui::PlotLines("Scope", scope.data(), scope.size(), 0, "", -1, 1, ImVec2(0,80));
        // ImGui::End();

        ImGui::Begin("Drum 1");
        drum1.drawGui();
        ImGui::End();

        ImGui::Begin("Drum 2");
        drum2.drawGui();
        ImGui::End();

        ab = powf(10, stringAmp / 10);
        as1 = powf(10, springAmp1 / 10);
        as2 = powf(10, springAmp2 / 10);
        a = powf(10, amp / 10);

        // ImGui::End();
    });

    rt.stop();
}