project "example_sdl2_sdlrenderer2_custom"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    debugdir "%{prj.location}"

    targetdir ("%{prj.location}/" .. build_bin_dir .. "/" .. outputdir)
    objdir ("%{prj.location}/" .. build_int_dir .."/" .. outputdir)

    files
    {
        "%{prj.location}/main.cpp"
    }

    includedirs
    {
        "%{wks.location}/../../SDL/include",
        "%{wks.location}/..",
        "%{wks.location}/../backends"
    }

    libdirs
    {
        "%{wks.location}/../../SDL/build/bin/" .. outputdir,
        "%{wks.location}/../../SDL/SDLmain/build/bin/" .. outputdir,
        "%{wks.location}/../build/bin/" .. outputdir
    }

    links
    {
        "SDL",
        "SDLmain",
        "imgui"
    }

    buildoptions
    {
        "/utf-8"
    }

        -- TODO: Clarify how to copy system specific files. Currently only Windows supported
    postbuildcommands
    {
        ("{COPY} %{wks.location}/../../SDL/" .. build_bin_dir .. "/" .. outputdir .. "/SDL.dll %{prj.location}" .. build_bin_dir .. "/" .. outputdir),
        ("{COPY} %{wks.location}/../../SDL/" .. build_bin_dir .. "/" .. outputdir .. "/SDLmain.dll %{prj.location}" .. build_bin_dir .. "/" .. outputdir)
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
            "gdi32.lib"
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

