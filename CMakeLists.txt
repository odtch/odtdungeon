cmake_minimum_required(VERSION 3.5)

project(odtdungeon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src .)
file(GLOB source_files CONFIGURE_DEPENDS
    "README.md"
    "src/*.h"
    "src/*.cpp"
    "src/*/*.cpp"
    "src/*/*.h"
    "src/*/*/*.cpp"
    "src/*/*/*.h"
	#"src/vulkan/rasterizer/*.glsl"
	#"src/vulkan/raycompute/*.glsl"
	#"src/vulkan/raytracer/shader/*"
	"src/vulkan/raytracer.glsl/*"
	#"src/vulkan/raytracer/sensorshader/*"
)

include_directories(
	external/JoltPhysics
)
add_executable(odtdungeon ${source_files})

target_compile_definitions(odtdungeon PUBLIC "$<$<CONFIG:DEBUG>:ODTDEBUG>"
	_DEBUG
	ODTJOLT
	JPH_PROFILE_ENABLED # auch im release
	JPH_DEBUG_RENDERER
)

target_link_libraries(odtdungeon
    glfw
    #pthread
    vulkan
	assimp
	${CMAKE_CURRENT_SOURCE_DIR}/external/JoltPhysics/Build/Linux_Debug/libJolt.a
	SDL2
)

