workspace "LilEngie2"
	configurations {"Debug", "Release", "EditorGameDebug", "EditorGameRelease"}
	architecture "x86_64"
	startproject "LilEddie"

project "LilEngie"

	configmap {
		["EditorGameDebug"] = "Debug",
		["EditorGameRelease"] = "Release"
	}

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

	filter "configurations:Debug or EditorGameDebug"
		defines "LIL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release or EditorGameRelease"
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

	filter "configurations:Debug or EditorGameDebug"
		defines "LIL_DEBUG"
		runtime "Debug"
		symbols "on"

		libdirs 
		{
			"Dependencies/Assimp/lib/Debug/x64/"
		}

	filter "configurations:Release or EditorGameRelease"
		runtime "Release"
		optimize "on"

		libdirs 
		{
			"Dependencies/Assimp/lib/Release/x64/"
		}

	filter "configurations:EditorGameDebug or EditorGameRelease"
		kind "SharedLib"

	filter "system:windows"
		systemversion "latest"

project "LilEddie"
	configmap {
		["EditorGameDebug"] = "Debug",
		["EditorGameRelease"] = "Release"
	}

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
		"copy /Y \"%{wks.location}Dependencies\\Assimp\\bin\\%{cfg.buildcfg}\\x64\\assimp-vc142-mt.dll\" \"%{prj.location}bin\\%{cfg.buildcfg}\\x64\\assimp-vc142-mt.dll\"",
	}

	filter "configurations:Debug or EditorGameDebug"
		defines "LIL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release or EditorGameRelease"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		systemversion "latest"

project "LilBuilder"
	configmap {
		["EditorGameDebug"] = "Debug",
		["EditorGameRelease"] = "Release"
	}

	location "Tools/%{prj.name}"
	kind "ConsoleApp"
	language "C#"

	files { "%{prj.location}/src/**.cs" }

	links{
		"System",
		"System.Core",
		"System.IO",
		"System.Diagnostics"
	}

	targetdir "%{prj.location}/bin/%{cfg.buildcfg}/x64/"
	objdir "%{prj.location}/bin/int/%{cfg.buildcfg}/x64/"

	filter "configurations:Debug or EditorGameDebug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release or EditorGameRelease"
		runtime "Release"
		optimize "on"
