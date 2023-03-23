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

    targetdir "%{cfg.buildcfg}"
    objdir "%{cfg.buildcfg}/int/"

    includedirs
    {
        "libs/glfw/include/",
        "src/glad/include/",
        "libs/glm/"
    }

    files
    {
        "src/**.cpp",
        "src/**.c",
        "src/**.h"
    }

    filter { "system:windows" }
        libdirs {glfw_dir .. "/lib-vc2022"}
        links { "glfw3", "OpengL32", "glm" }
    filter { "system:macosx" }
        libdirs {glfw_dir}
        links { "glfw3", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework", "OpenGL.framework", "glm" }

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