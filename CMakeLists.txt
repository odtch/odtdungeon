cmake_minimum_required(VERSION 3.5)

project(odtdungeon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src .)
file(GLOB source_files CONFIGURE_DEPENDS
			"src/*.h"
			"src/*.cpp"
			"src/*/*.cpp"
			"src/*/*.h"
			"src/*/*/*.cpp"
			"src/*/*/*.h"
			"src/vulkan/rasterizer/*.glsl"
			"src/vulkan/raycompute/*.glsl"
			"src/vulkan/raytracer/shader/*"
			"src/vulkan/raytracer/sensorshader/*"
	)

include_directories(
)
add_executable(odtdungeon ${source_files})

target_compile_definitions(odtdungeon PUBLIC "$<$<CONFIG:DEBUG>:ODTDEBUG>"
	_DEBUG
)

target_link_libraries(odtdungeon
)

