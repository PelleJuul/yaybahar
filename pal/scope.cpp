#include "scope.h"
#include "imgui/imgui.h"

Scope::Scope(int l) :
    data(l, 0)
{

}

void Scope::write(float x)
{
    if (freeze)
    {
        return;
    }

    if (triggerEnabled && x > trigger && prev < trigger)
    {
        for (int i = 0; i < data.size(); i++)
        {
            data[i] = 0;
        }
    }
    else
    {
        for (int i = 1; i < data.size(); i++)
        {
            data[i-1] = data[i];
        }
    }
    
    data[data.size() - 1] = x;
    prev = x;
}

void Scope::draw()
{
    ImGui::PlotLines("Scope", data.data(), data.size(), 0, "", -1, 1, ImVec2(0,80));

    /*
    ImGui::SliderFloat("trigger label", &trigger, -1, 1);
    ImGui::Checkbox("trigger enabled", &triggerEnabled);
    */
    ImGui::Checkbox("Freeze", &freeze);
}