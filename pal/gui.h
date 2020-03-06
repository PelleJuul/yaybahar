#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl2.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <functional>

// This is a test

class Gui
{
    public:
    virtual void layout() = 0;
    void show();

    protected:
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

class QuickGui : public Gui
{
    public:
    QuickGui(std::function<void()> layoutFunction);
    void layout();

    private:
    std::function<void()> layoutFunction;
};

void quickGui(std::function<void()> layout);