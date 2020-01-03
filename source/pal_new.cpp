#include <pal.h>

int main(int argc, char **argv)
{
    Realtime rt = quickAudio([&](int n, int nc, float *in, float *out)
    {
        for (int i = 0; i < n; i++)
        {
            // Compute your sample here.
            float y = 0;

            for (int c = 0; c < nc; c++)
            {
                // Write it to the output array here.
                out[i * nc + c] = y;
            }
        }
    });

    quickGui([&]()
    {
        ImGui::Begin("New PAL project");
        ImGui::Text("Hello, world!");
        ImGui::End();
    });

    rt.stop();

    return 0;
}