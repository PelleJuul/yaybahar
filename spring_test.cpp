#include <pal.h>
#include "spring.h"

int main(int argc, char **argv)
{
    Spring spring(100, 44100);
    bool strike = false;

    Realtime rt = quickAudio([&](int n, float x)
    {
        if (strike)
        {
            strike = false;
            spring.addForce(20, 50000);
        }

        float y = spring.computeNextSample(0);
        return 1e3 * y;
    });

    quickGui([&]()
    {
        ImGui::Begin("New PAL project");
        if (ImGui::Button("Strike"))
        {
            strike = true;
        }
        spring.drawGui();
        ImGui::End();
    });

    rt.stop();
}