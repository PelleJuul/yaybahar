#include <stdio.h>
#include <cmath>
#include <vector>
#include <pal.h>
#include "BowedString.h"
#include "Spring.h"
#include "Connection.h"
#include "RadialPlate.h"

// pal build Yaybahar.cpp BowedString.cpp Connection.cpp LineDomain.cpp RadialDomain.cpp Spring.cpp Domain2d.cpp -O3 -o yaybahar

int main(int argc, char **argv)
{
    float fs = 44100;
    float amp = 0;
    float stringAmp = 0.0;
    float springAmp1 = 0.0;
    float springAmp2 = 0.0;
    Gain plate1Gain("Plate 1");
    Gain plate2Gain("Plate 2");

    BowedString bowedString(55, fs);
    Spring spring1(250, fs);
    Spring spring2(250, fs);

    RadialPlate<5, 13> plate1(44100);
    RadialPlate<5, 13> plate2(44100);
    plate1.setWavespeed(75);
    plate2.setWavespeed(150);

    Connection c1(44100);
    Connection c2(44100);
    Connection c11(44100);
    Connection c22(44100);

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
            float f1 = c1.calculateForce(bowedString.at(10), spring1.at(0));
            float f2 = c2.calculateForce(bowedString.at(10), spring2.at(0));
            float f11 = c11.calculateForce(
                spring1.at(spring1.size() - 1),
                plate1.get(3, 0)
            );
            float f22 = c22.calculateForce(
                spring2.at(spring2.size() - 1),
                plate2.get(3, 0)
            );

            bowedString.addForce(10, f1);
            bowedString.addForce(10, f2);

            // printf("%.2f\n", f1);

            spring1.addForce(0, -f1);
            spring1.addForce(spring1.size() - 1, 10 * f11);

            spring2.addForce(0, -f2);
            spring2.addForce(spring2.size() - 1, 10 * f22);

            plate1.addForce(3, 0, -f11);
            plate2.addForce(3, 0, -f22);

            float b = bowedString.getNextSample();
            float s1 = spring1.computeNextSample(0);
            float s2 = spring2.computeNextSample(0);

            plate1.calculate();
            plate2.calculate();


            float p1 = plate1Gain * plate1.get(2, 0);
            float p2 = plate2Gain * plate2.get(2, 0);

            // printf("%f\n", p1);

            float y = a * 1e3 * (ab * b + as1 * s1 + as2 * s2 + p1 + p2);
            
            for (int c = 0; c < nc; c++)
            {
                // Write it to the output array here.
                out[i * nc + c] = y;
            }
        }
    });

    quickGui([&]()
    {
        bowedString.drawGui();

        ImGui::Begin("String 1");
        spring1.drawGui();
        ImGui::End();

        ImGui::Begin("String 2");
        spring2.drawGui();
        ImGui::End();

        ImGui::Begin("Mixer");
        ImGui::LabelText("CPU load", "%.2f", rt.getCpuLoad());
        ImGui::SliderFloat("String", &stringAmp, -30, 3);
        ImGui::SliderFloat("Spring 1", &springAmp1, -30, 3);
        ImGui::SliderFloat("Spring 2", &springAmp2, -30, 3);
        plate1Gain.draw();
        plate2Gain.draw();
        ImGui::SliderFloat("Master", &amp, -30, 3);
        // ImGui::PlotLines("Scope", scope.data(), scope.size(), 0, "", -1, 1, ImVec2(0,80));
        ImGui::End();

        ImGui::Begin("Plate 1");
        plate1.draw();
        ImGui::End();

        ImGui::Begin("Plate 2");
        plate2.draw();
        ImGui::End();

        ab = powf(10, stringAmp / 10);
        as1 = powf(10, springAmp1 / 10);
        as2 = powf(10, springAmp2 / 10);
        a = powf(10, amp / 10);

        // ImGui::End();
    });

    rt.stop();
}