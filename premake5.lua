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

    targetdir "bin/RayTracing/%{cfg.buildcfg}"
    objdir "bin-int/RayTracing/%{cfg.buildcfg}"

    includedirs
    {
        "libs/glfw/include/",
        "src/glad/include/"
    }

    files
    {
        "src/**.cpp",
        "src/**.c",
        "src/**.h"
    }

    filter { "system:windows" }
        libdirs {glfw_dir .. "/lib-vc2022"}
        links { "glfw3", "OpengL32" }
    filter { "system:macosx" }
        libdirs {glfw_dir}
        links { "glfw3", "GLAD", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework", "OpenGL.framework" }


