-- Set up GLFW and GLEW directory paths
glfw_dir = "libs/glfw/"
glad_dir = "libs/glad/"

-- Define the project
workspace "MyProject"
    configurations { "Debug", "Release" }
    architecture "x86_64"

-- Set up the C++ project
project "RayTracingIOW"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/obj/%{cfg.buildcfg}"

    -- Add source files here
    files { "src/**.cpp" }

    -- Include GLFW and GLEW headers
    includedirs { glfw_dir, glad_dir .. "/include" }

    -- Link GLFW and GLEW libraries
    filter { "system:windows" }
        links { "glfw3", "glew32s", "OpengL32" }
    filter { "system:macosx" }
        libdirs {glfw_dir}
        defines {GL_SILENCE_DEPRECATION}
        links { "glfw3", "GLAD", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework", "OpenGL.framework" }

    -- Set compiler and linker options
    filter { "system:windows" }
        cppdialect "C++17"
        systemversion "latest"
        staticruntime "on"
        buildoptions { "/permissive-", "/W4", "/w14640" }
        flags { "MultiProcessorCompile" }
    filter { "system:macosx" }
        cppdialect "C++17"
        buildoptions { "-Wall", "-Wextra", "-Wpedantic" }
        linkoptions { "-stdlib=libc++" }

    -- Set debug and release configurations
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

include "libs/glad/gladPremake5.lua"