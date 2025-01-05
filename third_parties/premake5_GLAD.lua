project "GLAD"
    location "%{wks.location}/third_parties/GLAD"
    kind "StaticLib"
    language "C"
    cdialect "C17"
    staticruntime "off"

    targetdir ("%{prj.location}/build/bin/" .. outputdir)
    objdir ("%{prj.location}/build/intermediate/" .. outputdir)

    files
    {
        "%{prj.location}/include/glad/glad.h",
        "%{prj.location}/include/KHR/khrplatform.h",
        "%{prj.location}/glad.c"
    }

    includedirs
    {
        "%{prj.location}/include"
    }

    defines
    {
        --"GLAD_GLAPI_EXPORT",
        --"GLAD_GLAPI_EXPORT_BUILD"
    }

    filter "system:windows"
        systemversion = "latest"

        defines
        {
            "_WIN32"
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
