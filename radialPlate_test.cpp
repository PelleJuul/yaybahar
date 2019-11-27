#include <pal.h>
#include <vector>
#include <cmath>
#include "RadialPlate.h"

#define sqr(x)   (x * x)

int main(int argc, char **argv)
{
    RadialPlate<6, 18> plate(44100.0);

    float max = 0;

    Realtime rt = quickAudio([&](int n, float x)
    {
        plate.calculate();
        float y = 2e3 * plate.get(3, 0);

        if (fabs(y) > 1.0)
        {
            printf("Instability detected. Stopping audio.\n");
            rt.stop();
            return 0.0f;
        }

        return y;
    });

    quickGui([&]()
    {
        ImGui::ShowDemoWindow();
        ImGui::Begin("Radial Plate Test");
        
        if (ImGui::Button("Strike"))
        {
            printf("Striking plate\n");
            plate.addForce(0, 0, 5000);
        }

        ImGui::End();
    });

    rt.stop();

    return 0;
}