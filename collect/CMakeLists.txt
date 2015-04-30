cmake_minimum_required(VERSION 2.8.8)
project(magneto-collect)

include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tiva)

CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)


# Tivaware
set(TIVAWARE_PATH "/home/luck/.local/share/tivaware")
add_library(tivaware SHARED IMPORTED)
set_property(
	TARGET tivaware
	PROPERTY IMPORTED_LOCATION "${TIVAWARE_PATH}/driverlib/gcc/libdriver.a"
)
set_property(
	TARGET tivaware
	PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${TIVAWARE_PATH}"
)


# Source files
set(collect_src
	src/buttons.c
	src/io.c
	src/magneto.c
	src/main.c
	src/rgb_led.c
	src/startup_gcc.c
	src/timer.c
	src/uart.c
)

add_executable(collect
	"${collect_src}"
)
set_target_properties(collect
	PROPERTIES OUTPUT_NAME bin/main.elf
)

target_compile_options(collect PUBLIC
	"-std=c99"
	"-Wall"
	"-Wextra"
	"-pedantic"

	"-fdata-sections"
	"-ffunction-sections"

	"-g"
)

target_compile_options(collect PUBLIC
	"-mcpu=cortex-m4"
	"-mthumb"
	"-mfpu=fpv4-sp-d16"
	"-mfloat-abi=softfp"
)

target_compile_definitions(collect PUBLIC
	"PART_TM4C123GH6PM"
	"TARGET_IS_TM4C123_RB1"
)

target_link_libraries(collect PUBLIC
	tivaware
)

# Create binary
add_custom_command(
	TARGET collect
	POST_BUILD
	WORKING_DIRECTORY bin
	COMMAND arm-none-eabi-objdump --disassemble main.elf > main.asm
	COMMAND arm-none-eabi-objcopy -O srec main.elf main.s19
	COMMAND arm-none-eabi-objcopy -O binary main.elf main.bin
)

# Linker options
set(LDSCRIPT_PATH "ldscript.ld")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_EXE_LINKER_FLAGS
	"${CMAKE_EXE_LINKER_FLAGS}"
	"-T${LDSCRIPT_PATH}"
	"-Wl,--gc-sections"
)
string(REGEX REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "")
