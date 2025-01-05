project "glm"
    location "%{wks.location}/third_parties/glm"
    kind "None" -- Actually this whole project file is a stub, I used it just to identify glm to be listed in dependent project's list

    files
    {
        "%{prj.location}/glm/**.h",
        "%{prj.location}/glm/**.hpp",
        "%{prj.location}/glm/**.inl"
    }
    