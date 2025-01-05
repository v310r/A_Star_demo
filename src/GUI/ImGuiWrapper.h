#pragma once


struct SDL_Window;

class ImGuiWrapper
{
public:
    static void Init(SDL_Window* window, const char* glslVersion);

    static void NewFrame();

    static void EndFrameAndRender();

    static void Cleanup();

    static ImGuiIO* GetIO();

protected:
    static ImGuiIO* s_IO;
};
