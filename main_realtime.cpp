#include <stdio.h>
#include <cmath>
#include <vector>
#include <pal.h>
#include "bowedstring.h"
#include "spring.h"
#include "waveguide.h"

int main(int argc, char **argv)
{
    float fs = 44100;
    float amp = 0;
    float stringAmp = 0.0;
    float springAmp1 = 0.0;
    float springAmp2 = 0.0;
    BowedString bowedString(100, fs);
    Spring spring1(100, fs);
    Spring spring2(100, fs);
    Waveguide wg(ceil(fs * (1.0 / 220.0)) + 1);

    bowedString.Fb = 0;

    std::vector<float> scope(1000, 0);
    bool impulse = false;

    Realtime rt = quickAudio([&](int n, float x)
    {
        float b = bowedString.getNextSample();
        float s1 = spring1.computeNextSample(b);
        float s2 = spring2.computeNextSample(b);

        float ab = powf(10, stringAmp / 10);
        float as1 = powf(10, springAmp1 / 10);
        float as2 = powf(10, springAmp2 / 10);
        float a = powf(10, amp / 10);

        for (int i = 1; i < scope.size(); i++)
        {
            scope[i-1] = scope[i];
        }

        // float l = fs * (1.0 / 220);
        // float fb = 0.99 * wg.read(l);

        // if (impulse)
        // {
        //     wg.write(fb + 0.5);
        //     impulse = false;
        // }
        // else
        // {
        //     wg.write(fb);
        // }
        
        float y = a * 1e3 * (ab * b + as1 * s1 + as2 * s2);
        // scope[scope.size() - 1] = y;
        return y;
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
        ImGui::SliderFloat("String", &stringAmp, -30, 3);
        ImGui::SliderFloat("Spring 1", &springAmp1, -30, 3);
        ImGui::SliderFloat("Spring 2", &springAmp2, -30, 3);
        ImGui::SliderFloat("Master", &amp, -30, 3);
        // ImGui::PlotLines("Scope", scope.data(), scope.size(), 0, "", -1, 1, ImVec2(0,80));
        ImGui::End();

        // ImGui::Begin("Waveguide");

        // if (ImGui::Button("Strike"))
        // {
        //     impulse = true;
        // }

        // // ImGui::PlotLines("Data", wg.data(), wg.size(), 0, "", -1, 1, ImVec2(0,80)); 

        // ImGui::End();
    });

    rt.stop();
}