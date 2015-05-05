cmake_minimum_required (VERSION 2.8.8)
project (magneto-process C CXX)

add_subdirectory("../libprocess" "libprocess")

set(engine_src
	src/core/Engine.cpp
	src/core/Handlers.cpp
	src/core/Window.cpp
	src/module/Font.cpp
	src/module/Log.cpp
	src/module/TextureMap.cpp
)

set(visualize_src
	src/main.cpp
	src/Visualize.cpp
)

add_executable(visualize
	"${engine_src}"
	"${visualize_src}"
)

target_compile_options(visualize PRIVATE
	"-std=c++11"

	"-Wall"
	"-Wextra"
	"-pedantic"

	"-fdata-sections"
	"-ffunction-sections"
)

target_link_libraries(visualize libprocess)
target_link_libraries(visualize
	"-lSDL2"
	"-lSDLmain"
	"-lSDL2_image"
	"-lSDL2_ttf"
	"-lSDL2_gfx"
)
target_include_directories(visualize PRIVATE "${CMAKE_SOURCE_DIR}/src")
