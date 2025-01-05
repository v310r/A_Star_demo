project "imgui"
    location "%{wks.location}/third_parties/imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/*.h",
        "%{prj.location}/*.cpp",
        "%{prj.location}/backends/imgui_impl_sdl2.cpp",
        "%{prj.location}/backends/imgui_impl_sdl2.h",
        "%{prj.location}/backends/imgui_impl_sdlrenderer2.cpp",
        "%{prj.location}/backends/imgui_impl_sdlrenderer2.h",
        "%{prj.location}/backends/imgui_impl_opengl3.cpp",
        "%{prj.location}/backends/imgui_impl_opengl3.h",
        "%{prj.location}/backends/imgui_impl_opengl3_loader.h"
    }

    includedirs
    {
        "%{prj.location}",
        "%{wks.location}/third_parties/SDL/include"
    }

    filter "system:windows"
        systemversion = "latest"

        filter "configurations:Debug"
            symbols "On"
            defines
            {
                "_DEBUG"
            }
        
        filter "configurations:Release"
            optimize "On"
            defines
            {
                "NDEBUG"
            }
