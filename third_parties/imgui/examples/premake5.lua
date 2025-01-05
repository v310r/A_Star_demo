workspace "imgui_examples_custom"
    startproject "example_sdl2_sdlrenderer2_custom"

    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    build_bin_dir = "build/bin"
    build_int_dir = "build/intermediate"
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

   
    dofile("example_sdl2_sdlrenderer2/premake5.lua")

