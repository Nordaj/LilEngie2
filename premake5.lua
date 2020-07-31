workspace "LilEngie2"
	configurations {"Debug", "Release"}
	architecture "x86_64"
	startproject "LilEddie"

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
		"Dependencies/Assimp/lib/%{cfg.buildcfg}/x64/"
	}

	links
	{
		"glew32s.lib",
		"assimp-vc142-mt.lib",
		"IrrXML.lib",
		"zlibstatic.lib",
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
	kind "SharedLib"
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
		"Dependencies/Assimp/lib/%{cfg.buildcfg}/x64/"
	}

	links
	{
		"glew32s.lib",
		"assimp-vc142-mt.lib",
		"IrrXML.lib",
		"zlibstatic.lib",
		"glew32s.lib",
		"D3D11.lib",
		"D3DCompiler.lib",
		"opengl32.lib",
		"LilEngie"
	}

	postbuildcommands{
		"copy /Y \"%{wks.location}Dependencies\\Assimp\\bin\\%{cfg.buildcfg}\\x64\\assimp-vc142-mt.dll\" \"%{prj.location}bin\\%{cfg.buildcfg}\\x64\\assimp-vc142-mt.dll\""
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

project "LilEddie"
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
		"Dependencies/Assimp/lib/%{cfg.buildcfg}/x64/"
	}

	links
	{
		"glew32s.lib",
		"assimp-vc142-mt.lib",
		"IrrXML.lib",
		"zlibstatic.lib",
		"glew32s.lib",
		"D3D11.lib",
		"D3DCompiler.lib",
		"opengl32.lib",
		"LilEngie"
	}

	postbuildcommands{
		"copy /Y \"%{wks.location}Dependencies\\Assimp\\bin\\%{cfg.buildcfg}\\x64\\assimp-vc142-mt.dll\" \"%{prj.location}bin\\%{cfg.buildcfg}\\x64\\assimp-vc142-mt.dll\""
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
