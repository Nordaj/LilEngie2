workspace "LilEngie2"
	configurations {"Debug", "Release"}
	architecture "x86_64"
	startproject "Sandbox"

project "LilEngie"
	location "%{prj.name}"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir "%{prj.name}/bin/%{cfg.buildcfg}/x64/"
	objdir "%{prj.name}/bin/int/%{cfg.buildcfg}/x64/"

	--TODO: remove later, this is vs only but wont compile without due to win api stuff
	characterset "MBCS"

	files 
	{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"LIL_WINDOWS",
		"GLEW_STATIC",
		"LIL_ENABLE_IMGUI"
	}

	includedirs
	{
		"%{prj.name}/src/",
		"Dependencies/GLEW/include/",
		"Dependencies/Assimp/include/",
		"Dependencies/nlohmannJSON/include/",
		"Dependencies/stb/include/"
	}

	libdirs
	{
		"Dependencies/GLEW/lib/Release/x64/",
		"Dependencies/Assimp/lib/x64/"
	}

	links
	{
		"glew32s.lib",
		"assimp-vc141-mt_s.lib",
		"glew32s.lib",
		"D3D11.lib",
		"D3DCompiler.lib",
		"opengl32.lib"
	}

	filter "configurations:Debug"
		defines "LIL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		systemversion "latest"

project "Sandbox"
	location "%{prj.name}"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir "%{prj.name}/bin/%{cfg.buildcfg}/x64/"
	objdir "%{prj.name}/bin/int/%{cfg.buildcfg}/x64/"

	files 
	{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"LIL_WINDOWS",
		"GLEW_STATIC",
		"LIL_ENABLE_IMGUI"
	}

	includedirs
	{
		"LilEngie/src/",
		"%{prj.name}/src/",
		"Dependencies/GLEW/include/",
		"Dependencies/Assimp/include/",
		"Dependencies/nlohmannJSON/include/",
		"Dependencies/stb/include/"
	}

	libdirs
	{
		"Dependencies/GLEW/lib/Release/x64/",
		"Dependencies/Assimp/lib/x64/"
	}

	links
	{
		"glew32s.lib",
		"assimp-vc141-mt_s.lib",
		"glew32s.lib",
		"D3D11.lib",
		"D3DCompiler.lib",
		"opengl32.lib",
		"LilEngie"
	}

	filter "configurations:Debug"
		defines "LIL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		systemversion "latest"
