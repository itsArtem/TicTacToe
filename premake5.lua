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
		"Source/**.h",
		"Source/**.cpp",
		"Resources/**",
		"Dependencies/**",
		"Tools/**",
		"**.md",
		"**.lua",
		".gitignore",
		"LICENSE"
	}
	
	vpaths
	{
		["Header Files"] = "**.h",
		["Source Files"] = "**.cpp",
		["Resource Files"] = {"**.png", "**.ogg", "**.wav"}
	}

	includedirs
	{
		"Dependencies/SDL2/include/",
		"Dependencies/SDL2_image/include/",
		"Dependencies/SDL2_ttf/include/",
		"Dependencies/SDL2_mixer/include/",
		"Dependencies/GSL/include/"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib",
		"SDL2_mixer.lib"
	}

	-- Configs

	filter "configurations:Debug"
		kind "ConsoleApp"
		symbols "On"
		defines "TTT_DEBUG"

	filter "configurations:Release"
		kind "WindowedApp"
		optimize "On"
		defines "TTT_RELEASE"
	
	-- Platforms

	filter "platforms:x86"
		targetdir "Output/%{cfg.buildcfg}/x86/"
		objdir "Output/Intermediates/%{cfg.buildcfg}/x86/"

		libdirs
		{
			"Dependencies/SDL2/lib/x86/",
			"Dependencies/SDL2_image/lib/x86/",
			"Dependencies/SDL2_ttf/lib/x86/",
			"Dependencies/SDL2_mixer/lib/x86/"
		}

	filter "platforms:x64"
		targetdir "Output/%{cfg.buildcfg}/x64/"
		objdir "Output/Intermediates/%{cfg.buildcfg}/x64/"

		libdirs
		{
			"Dependencies/SDL2/lib/x64/",
			"Dependencies/SDL2_image/lib/x64/",
			"Dependencies/SDL2_ttf/lib/x64/",
			"Dependencies/SDL2_mixer/lib/x64/"
		}