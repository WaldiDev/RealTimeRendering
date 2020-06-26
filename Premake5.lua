workspace "RealTimeRendering"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Shipping"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["ImGui"] = "RTREngine/ThirdParty/Imgui"

group "Dependencies"
	include "RTREngine/ThirdParty/ImGui"

project "RTREngine"
	location "RTREngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{prj.name}/ThirdParty/Spdlog/include",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RTR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTR_RELEASE"
		runtime "Release"		
		optimize "on"

	filter "configurations:Shipping"
		defines "RTR_SHIPPING"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"RTREngine/Source",
		"RTREngine/ThirdParty/Spdlog/include",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"RTREngine"
	}

	filter "system:windows"		
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RTR_DEBUG"
		runtime "Debug"		
		symbols "on"

	filter "configurations:Release"
		defines "RTR_RELEASE"
		runtime "Release"		
		optimize "on"

	filter "configurations:Shipping"
		defines "RTR_SHIPPING"
		runtime "Release"		
		optimize "on"