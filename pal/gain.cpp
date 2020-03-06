#include "gain.h"
#include "imgui/imgui.h"

Gain::Gain(std::string name)
{
    this->name = name;
}

void Gain::draw()
{
    ImGui::SliderFloat(name.c_str(), &gain, -32, 6);
}