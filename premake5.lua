workspace "PDI"
    architecture "x64"
    startproject "Tecodity"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

group "Dependencies"

group ""

project "Tecodity"
    location "Tecodity"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"    

    targetdir("bin/" .. outputdir .. "/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Tecodity/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
    }

    links
    {
    }    

    filter "system:windows"        
        systemversion "latest"

        links 
		{ 
		}

        defines
        {
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../data/\"")
        }

    filter "configurations:Debug"
        defines "TECODITY_DEBUG"
        runtime "Debug"        
        symbols "on"

    filter "configurations:Release"
        defines "TECODITY_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "TECODITY_DIST"
        runtime "Release"
        optimize "on"