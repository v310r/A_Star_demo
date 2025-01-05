project "SDLmain"
    location "%{_SCRIPT_DIR}"
    kind "SharedLib"
    language "C"
    cdialect "C17"
    staticruntime "off"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/" .. SDL_build_bin_dir .. "/" .. SDL_outputdir)
    objdir ("%{prj.location}/" .. SDL_build_int_dir .. "/" .. SDL_outputdir)

    includedirs
    {
        "%{wks.location}/third_parties/SDL/include"
    }

    files
    {
        "%{prj.location}/src/SDLmain.c"
    }

    -- TODO: Clarify how to copy system specific files. Currently only Windows supported
    postbuildcommands 
    {
        ("{MKDIR} %{prj.location}/../" .. SDL_build_bin_dir .. "/" .. SDL_outputdir),
        ("{COPY} %{prj.location}" .. SDL_build_bin_dir .. "/" .. SDL_outputdir .. "/SDLmain.dll %{prj.location}/../" .. SDL_build_bin_dir .. "/" .. SDL_outputdir)
    }

    filter "system:windows"
        systemversion = "latest"

        defines
        {
            "__WIN32__",
            "_WINDOWS",
            "UNICODE",
            "_UNICODE",
            "_CRT_SECURE_NO_WARNINGS"
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
            