cmake_minimum_required (VERSION 2.8.8)
project (magneto-process C CXX)

add_subdirectory("../libprocess" "libprocess")

set(process_src
	src/main.cpp
	src/serial.cpp
)

add_executable(process "${process_src}")

target_compile_options(process PRIVATE
	"-std=c++11"

	"-Wall"
	"-Wextra"
	"-pedantic"

	"-fdata-sections"
	"-ffunction-sections"
)

target_link_libraries(process libprocess)
