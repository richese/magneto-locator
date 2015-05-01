cmake_minimum_required(VERSION 2.8.8)
project(magneto-collect)

include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tiva)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

#
# Set correct compiler
#
find_program(ARM-CC arm-none-eabi-gcc)
find_program(ARM-CXX arm-none-eabi-g++)
CMAKE_FORCE_C_COMPILER("${ARM-CC}" GNU)
CMAKE_FORCE_CXX_COMPILER("${ARM-CXX}" GNU)



#
# Tivaware configuration
#
set(TIVAWARE_PATH CACHE PATH "Path to Tivaware installation base directory.")
set(LDSCRIPT_PATH "${TIVAWARE_PATH}/examples/boards/ek-tm4c123gxl/project0/project0.ld")
add_library(tivaware SHARED IMPORTED)
set_property(
	TARGET tivaware
	PROPERTY IMPORTED_LOCATION "${TIVAWARE_PATH}/driverlib/gcc/libdriver.a"
)
set_property(
	TARGET tivaware
	PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${TIVAWARE_PATH}"
)
set_property(
	TARGET tivaware
	PROPERTY INTERFACE_LINK_LIBRARIES "-T${LDSCRIPT_PATH}"
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

# Output name
set(OUTPUT_DIR "bin")
set(OUTPUT_BASENAME "${OUTPUT_DIR}/main")
set(ELF_PATH "${OUTPUT_BASENAME}.elf")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}")

# Output target
set_target_properties(collect
	PROPERTIES OUTPUT_NAME "${ELF_PATH}"
)

target_compile_options(collect PUBLIC
	"-std=c99"
	"-Wall"
	"-Wextra"
	"-pedantic"

	"-fdata-sections"
	"-ffunction-sections"

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
	"-Wl,--gc-sections"
	tivaware
)

#
# Rules to dissasemble and translate to binary.
#
find_program(ARM-OBJDUMP arm-none-eabi-objdump)
set(ASM_PATH "${OUTPUT_BASENAME}.asm")
add_custom_command(
	TARGET collect
	POST_BUILD
	COMMENT "Disassembling ${ELF_PATH} to ${ASM_PATH}"
	COMMAND "${ARM-OBJDUMP}" --disassemble "${CMAKE_BINARY_DIR}/${ELF_PATH}" > "${CMAKE_BINARY_DIR}/${ASM_PATH}"
)

find_program(ARM-OBJCOPY arm-none-eabi-objcopy)
set(BIN_PATH "${OUTPUT_BASENAME}.bin")
add_custom_command(
	TARGET collect
	POST_BUILD
	COMMENT "Translating ${ELF_PATH} to ${BIN_PATH}"
	COMMAND "${ARM-OBJCOPY}" -O binary "${CMAKE_BINARY_DIR}/${ELF_PATH}" "${CMAKE_BINARY_DIR}/${BIN_PATH}"

)

set_directory_properties(PROPERTIES
	ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/${BIN_PATH};${CMAKE_BINARY_DIR}/${ASM_PATH}"
)

#
# Rule to flash binary to chip.
#
# Udev rule to allow flashing without superuser permissions for members of group 'users':
#  echo 'ATTRS{idVendor}=="1cbe", ATTRS{idProduct}=="00fd", GROUP="users", MODE="0660"' | \
#          sudo tee /etc/udev/rules.d/99-stellaris-launchpad.rules
#
find_program(LM4FLASH_PATH lm4flash)
if (LM4FLASH_PATH STREQUAL "lm4flash-NOTFOUND")
else ()
	add_custom_target(flash
		COMMAND ${LM4FLASH_PATH} -v "${CMAKE_BINARY_DIR}${BIN_PATH}"
		DEPENDS collect
		COMMENT "Flashing binary."
	)
endif ()
