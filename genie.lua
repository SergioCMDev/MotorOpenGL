local project_list = {
	"AG01",
	"AG02",
	"AG03",
	"AG04",
	"AG05_1",
	"AG05_2",
	"AG06",
	"AG07_1",
	"AG07_2",
	"AG08_1",
	"AG08_2",
	"AG08_3",
	"AG08_4",
	"AG08_5",
	"AG09",
	"AG10_1",
	"AG10_2",
	"AG10_3",
	"AG11",
	"AG12",
	"AG13",
	"EJ2_1",
	"EJ2_2",
	"EJ2_3",
	"EJ2_4",
	"EJ2_5",
	"EJ2_6",
	"EJ3_1",
	"EJ3_2",
	"EJ3_3",
	"EJ3_4",
	"EJ4_1",
	"EJ4_2",
	"EJ4_5",
	"EJ5_1",
	"EJ5_3",
	"EJ5_4",
	"EJ5_5",
	"EJ5_6",
	"EJ5_7",
	"EJ6_1",
	"EJ6_2",
	"EJ6_4",
	"EJ6_5",
	"EJ7_1",
	"EJ7_2",
	"EJ7_3",
	"EJ8_2",
	"EJ8_3",

	
}

local function new_project(name)
	project(name)
	kind ("ConsoleApp")
	includedirs{"include", "src/deps/glfw", "src/deps/glad", "src/deps/stb"}
	files {	"tests/" ..name.. "/*.cpp",
			"tests/" ..name.. "/*.h",
	"src/*.cpp",
	"include/**.h",
	"src/deps/glfw/context.c",
	"src/deps/glfw/init.c",
	"src/deps/glfw/input.c",
	"src/deps/glfw/monitor.c",
	"src/deps/glfw/window.c",
	"src/deps/glfw/vulkan.c",
	"src/deps/glfw/glad.c",
	"src/deps/glfw/stb/*"}
	defines {"_GLFW_USE_OPENGL=1"}
	vpaths { ["Headers"] = "include/*.h",
			 ["Source"] = {"src/*.cpp"},
			 ["Deep Headers"] = {"include/GLFW/*",
						"include/glad/**",
						"include/KHR/**",
						"include/glm/**"},
		 	["Deps Srcs"] = "src/deps/**",
		 	["Tests"] = {"tests/"..name.. "/*.cpp",
					 	"tests/"..name.."/*.h"}}
	configuration{"debug"}
		flags{"Symbols"}
		targetsuffix("_d")
		libdirs { "libs/Debug" }
	configuration{"release"}
		flags{"Optimize"}
		targetsuffix("_r")
		libdirs { "libs/Release" }
	configuration{"windows"}
		files{"src/deps/glfw/egl_context.c",
			  "src/deps/glfw/win32*",
			  "src/deps/glfw/wgl_*",
			  "src/deps/glfw/winm_*",}
		links{"OpenGL32",  "assimp-vc140-mt"}
		defines{"_GLFW_WIN32", "_GLFW_WGL"}
		flags{"NoEditAndcontinue"}
		windowstargetplatformversion ="10.0.16299.0"
end
solution "04MVID_Class"
	configurations {"debug", "release"}
	language ("c++")
	platforms("x64")
	location("build")
	debugdir("build")
	targetdir("bin")
	flags{"ExtraWarnings"}
	for k,v in pairs(project_list) do
		new_project(v)
	end