glfw_dir = "libs/glfw/"

workspace "RayTracingWorkspace"
    configurations { "Debug", "Release" }
    startproject "RayTracing"
    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }

project "RayTracing"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    architecture "x64"
    dependson
    {
        "imgui", 
        "glm"
    }

    targetdir "%{cfg.buildcfg}"
    objdir "%{cfg.buildcfg}/int/"

    includedirs
    {
        "libs/glfw/include/",
        "src/glad/include/",
        "libs/glm/",
        "libs/imgui/",
        "libs/imgui/backends/"
    }

    files
    {
        "src/**.cpp",
        "src/**.c",
        "src/**.h"
    }

    filter { "system:windows" }
        libdirs {glfw_dir .. "/lib-vc2022"}
        links { "glfw3", "OpengL32", "glm", "imgui" }


project "glm"
    kind "StaticLib"
    language "C++"
    targetdir "%{cfg.buildcfg}"
    objdir "%{cfg.buildcfg}/int/"

    files
    {
        "libs/glm/glm/*.hpp",
        "libs/glm/glm/**.cpp"
    }

    includedirs
    {
        "libs/glm"
    }

project "imgui"
	kind "StaticLib"
	language "C"
	architecture "x86_64"

    targetdir "%{cfg.buildcfg}"
    objdir "%{cfg.buildcfg}/int/"
    
    includedirs 
    { 
        "libs/imgui/",
        "libs/imgui/backends/"
    }

	files 
    { 
        "libs/imgui/*.cpp", 
        "libs/imgui/*.h" ,
        "libs/imgui/backends/imgui_impl_opengl3.h",
        "libs/imgui/backends/imgui_impl_opengl3.cpp",
        "libs/imgui/backends/imgui_impl_glfw.h",
        "libs/imgui/backends/imgui_impl_glfw.cpp",
    }
    
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"