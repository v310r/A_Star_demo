project "spdlog"
    location "%{wks.location}/third_parties/spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{prj.location}/" .. build_bin_dir .. "/" .. outputdir)
    objdir ("%{prj.location}/" .. build_int_dir .."/" .. outputdir)

    files
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/include/**.h"
    }

    includedirs
    {
        "%{prj.location}/include"
    }

    defines
    {
        "SPDLOG_COMPILED_LIB"
    }

    buildoptions
    {
        "/utf-8"
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
