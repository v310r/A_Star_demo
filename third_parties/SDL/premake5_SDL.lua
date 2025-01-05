project "SDL"
    location "%{_SCRIPT_DIR}"
    kind "SharedLib"
    language "C"
    cdialect "C17"
    staticruntime "off"

    debugdir "%{prj.location}"

    SDL_location = "%{prj.location}"

    SDL_build_bin_dir = "build/bin"
    SDL_build_int_dir = "build/intermediate"
    SDL_outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    targetdir ("%{prj.location}/" .. SDL_build_bin_dir .. "/" .. SDL_outputdir)
    objdir ("%{prj.location}/" .. SDL_build_int_dir .. "/" .. SDL_outputdir)

    includedirs
    {
        "%{prj.location}/include",
        "%{prj.location}/src/internal_include" -- for easier include usage
    }

    files
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.c",
        "%{prj.location}/include/**.h"
    }

    links
    {
        "SDLmain"
    }

    filter "system:windows"
        systemversion = "latest"

        defines
        {
            "__WIN32__",
            "_WINDOWS",
            "UNICODE",
            "_UNICODE",
            "_CRT_SECURE_NO_WARNINGS",
            "DLL_EXPORT"
        }

        links
        {
            "Winmm"
        }

        filter "configurations:Debug"
            symbols "On"
            defines
            {
                "_DEBUG"
            }

            links
            {
                "Setupapi",
                "ucrtd",
                "vcruntimed",
            }
        
        filter "configurations:Release"
            optimize "On"
            defines
            {
                "NDEBUG"
            }

            links
            {
                "Setupapi",
                "ucrt",
                "vcruntime"
            }
    
            
    dofile("SDLmain/premake5_SDLmain.lua")
