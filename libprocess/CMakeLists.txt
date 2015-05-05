cmake_minimum_required (VERSION 2.8.8)
project(magneto-libprocess CXX)


set(libprocess_src
	src/fir.cpp
	src/geometry.cpp
	src/process.cpp
	src/shared.cpp
)

add_library(libprocess STATIC "${libprocess_src}")
set_target_properties(libprocess PROPERTIES OUTPUT_NAME "process")

target_compile_options(libprocess PUBLIC
	"-std=c++11"
)

target_compile_options(libprocess PRIVATE
	"-Wall"
	"-Wextra"
	"-pedantic"

	"-fdata-sections"
	"-ffunction-sections"
)

target_include_directories(libprocess INTERFACE "src")

target_link_libraries(libprocess "-lrt")
