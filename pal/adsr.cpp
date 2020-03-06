#include "adsr.h"
#include "imgui/imgui.h"
#include <cmath>

Adsr::Adsr(float a, float d, float s, float r, float fs)
{
    attack = a;
    decay = d;
    sustain = s;
    rel = r;
    this->fs = fs;
}

void Adsr::draw()
{
    {
        ImGui::SliderFloat("Attack", &attack, 0.0, 10.0);
        ImGui::SliderFloat("Decay", &decay, 0, 10.0);
        ImGui::SliderFloat("Sustain", &sustain, 0, 1);
        ImGui::SliderFloat("Release", &rel, 0, 10);
    }

    if (state != AdsrState::AdsrRelease)
    {
        if (ImGui::Button("Untrigger"))
        {
            release();
        }
    }
    else
    {
        if (ImGui::Button("Trigger"))
        {
            trigger();
        }
    }
    
}

float Adsr::next()
{
    if (state == AdsrState::AdsrAttack)
    {
        if (value >= 1.0)
        {
            state = AdsrState::AdsrDecay;
        }
        else
        {
            value += (1.0 / fs) * (1.0 / attack);
        }
    }

    if (state == AdsrState::AdsrDecay)
    {
        if (value <= sustain)
        {
            state = AdsrState::AdsrSustain;
        }
        else
        {
            value -= (1.0 / fs) * (1.0 / decay) * (1.0 - sustain);
        }
    }

    if (state == AdsrState::AdsrSustain && fabs(value - sustain) > 0.001)
    {
        if (value >= sustain)
        {
            value -= (1.0 / fs) * (1.0 / decay) * (1.0 - sustain);
        }
        else
        {
            value += (1.0 / fs) * (1.0 / attack);
        }
    }

    if (state == AdsrState::AdsrRelease)
    {
        if (value > 0)
        {
            value -= sustain * (1.0 / fs) * (1.0 / rel);
        }
    }

    if (value < 0)
    {
        value = 0;
    }
    
    return value;
}

void Adsr::release()
{
    state = AdsrState::AdsrRelease;
}

void Adsr::trigger()
{
    state = AdsrState::AdsrAttack;
}