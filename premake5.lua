workspace "A_Star_demo"
    startproject "A_Star_demo"

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

    project "A_Star_demo"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "off"

        debugdir "%{prj.location}"

        targetdir ("%{prj.location}/" .. build_bin_dir .. "/" .. outputdir)
        objdir ("%{prj.location}/" .. build_int_dir .."/" .. outputdir)

        files
        {
            "%{prj.location}/src/**.h",
            "%{prj.location}/src/**.cpp",
            "%{prj.location}/pch/pch.h",
            "%{prj.location}/pch/pch.cpp",
            "%{prj.location}/shaders/**"
        }

        includedirs
        {
            "%{prj.location}/src",
            "%{prj.location}/pch",
            "%{wks.location}/third_parties/SDL/include",
            "%{wks.location}/third_parties/GLAD/include",
            "%{wks.location}/third_parties/stb_image",
            "%{wks.location}/third_parties/glm/glm",
            "%{wks.location}/third_parties/imgui",
            "%{wks.location}/third_parties/imgui/backends",
            "%{wks.location}/third_parties/spdlog/include"
        }

        libdirs
        {
            "%{wks.location}/third_parties/SDL/build/bin/" .. outputdir,
            "%{wks.location}/third_parties/SDL/SDLmain/build/bin/" .. outputdir,
            "%{wks.location}/third_parties/GLAD/build/bin/" .. outputdir,
            "%{wks.location}/third_parties/stb_image/build/bin/" .. outputdir,
            "%{wks.location}/third_parties/imgui/build/bin/" .. outputdir,
            "%{wks.location}/third_parties/spdlog/build/bin/" .. outputdir
        }

        links
        {
            "SDL",
            "SDLmain",
            "stb_image",
            "GLAD",
            "imgui",
            "spdlog"
        }

        pchheader "pch.h"
        pchsource "pch/pch.cpp"

        buildoptions
        {
            "/utf-8"
        }

            -- TODO: Clarify how to copy system specific files. Currently only Windows supported
        postbuildcommands
        {
            ("{COPY} %{wks.location}third_parties/SDL/" .. build_bin_dir .. "/" .. outputdir .. "/SDL.dll %{prj.location}" .. build_bin_dir .. "/" .. outputdir),
            ("{COPY} %{wks.location}third_parties/SDL/" .. build_bin_dir .. "/" .. outputdir .. "/SDLmain.dll %{prj.location}" .. build_bin_dir .. "/" .. outputdir)
            --("{COPY} %{wks.location}third_parties/GLAD/" .. build_bin_dir .. "/" .. outputdir .. "/GLAD.dll %{prj.location}" .. build_bin_dir .. "/" .. outputdir)
        }

        filter "system:windows"
            systemversion = "latest"

            defines
            {
                "__WIN32__",
                "__WINDOWS__",
                "_CONSOLE"
            }
        
            links
            {
                "opengl32.lib",
                "kernel32.lib",
                "user32.lib",
                "gdi32.lib",
                "GLAD.lib"
            }

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
    
    dofile("third_parties/SDL/premake5_SDL.lua")

    dofile("third_parties/premake5_GLAD.lua")
    
    dofile("third_parties/premake5_imgui.lua")

    dofile("third_parties/premake5_glm.lua")

    dofile("third_parties/premake5_stb_image.lua")

    dofile("third_parties/premake5_spdlog.lua")
