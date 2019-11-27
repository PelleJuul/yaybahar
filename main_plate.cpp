#include <stdio.h>
#include <cmath>
#include <vector>
#include <pal.h>
#include "plate.h"

int main(int argc, char **argv)
{
    int w = 10;
    int h = 10;
    float fs = 44100;
    bool strike = false;
    Plate plate(w, h, 1.0 / h, fs);

    Realtime rt = quickAudio([&](int n, float x)
    {
        if (strike)
        {
            strike = false;
            plate.set(2, 2, 0.1);
        }

        plate.computeNextSample();
        return plate.get(6, 6);
    });

    quickGui([&]()
    {
        ImGui::Begin("Plate");
        ImGui::SliderFloat("Wavespeed", &plate.wavespeed, 10, 1000);

        if (ImGui::Button("Strike"))
        {
            strike = true;
        }
        ImGui::End();
    });

    return 0;
}