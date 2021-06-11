workspace "TicTacToe"
	configurations
	{
		"Debug",
		"Release"
	}
	
	platforms {"x86", "x64"}
	
	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"

project "TicTacToe"
	language "C++"
	cppdialect "C++17"
	systemversion "latest"

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Resources/**"
	}
	
	vpaths
	{
		["Header Files"] = "**.h",
		["Source Files"] = "**.cpp",
		["Resource Files"] = {"**.png", "**.json", "**.ogg", "**.wav"}
	}

	includedirs
	{
		"Dependencies/SDL2/include/",
		"Dependencies/SDL2_image/include/",
		"Dependencies/GSL/include/"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib"
	}

	-- Configs

	filter "configurations:Debug"
		kind "ConsoleApp"
		symbols "On"

	filter "configurations:Release"
		kind "WindowedApp"
		optimize "On"
	
	-- Platforms

	filter "platforms:x86"
		targetdir "%{prj.name}/Output/%{cfg.buildcfg}/x86/"
		objdir "%{prj.name}/Output/Intermediates/%{cfg.buildcfg}/x86/"

		libdirs
		{
			"Dependencies/SDL2/lib/x86/",
			"Dependencies/SDL2_image/lib/x86/"
		}

	filter "platforms:x64"
		targetdir "%{prj.name}/Output/%{cfg.buildcfg}/x64/"
		objdir "%{prj.name}/Output/Intermediates/%{cfg.buildcfg}/x64/"

		libdirs
		{
			"Dependencies/SDL2/lib/x64/",
			"Dependencies/SDL2_image/lib/x64/"
		}