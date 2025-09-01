---@diagnostic disable: undefined-global
workspace("Project")
    configurations { "Debug", "Release" }
    platforms { "x64", "x86" }
    language "C++"
    cppdialect "C++20"
    startproject "Server"

    targetdir "./build/vs/bin/%{cfg.buildcfg}"
    objdir "./build/vs/obj/%{cfg.buildcfg}"

    filter { "system:windows", "toolset:msc*" } 
        buildoptions { "/utf-8" }

project("Server")
    location "src"
    kind "ConsoleApp"

    files {
        "src/**.cpp",
        "src/**.h",
        "sec/**.hpp"
    }

    includedirs {
        "3rd/asio/asio/include",
    }

    filter "system:windows"
        links { "ws2_32.lib", "mswsock.lib", "advapi32.lib" }
    filter "system:linux"
        links { "pthread" }
