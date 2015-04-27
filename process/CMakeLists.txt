cmake_minimum_required (VERSION 2.8.8)
project (bakalarka-process)

# c++11 standard kôli STL
add_compile_options("-std=c++11")

## warningy
add_compile_options("-Wall" "-Wextra" "-pedantic")
add_compile_options("-fdata-sections" "-ffunction-sections")

## priečinok s hlavičkovými súbormi
include_directories(src)

## Release lib target
#add_library(process-lib STATIC
#    src/serial.cpp
#	src/process.cpp
#    src/geometry.cpp
#    )
#set_target_properties(process-lib PROPERTIES OUTPUT_NAME process)
#target_compile_options(process-lib PUBLIC "-O2" "-s")

## Debug lib target
add_library(process-lib-dbg STATIC
	src/serial.cpp
	src/process.cpp
	src/geometry.cpp
	src/shared_output.cpp
    )
set_target_properties(process-lib-dbg PROPERTIES OUTPUT_NAME process-dbg)
target_compile_options(process-lib-dbg PUBLIC "-g")

## Release target
#add_executable(process-bin main.cpp)
#set_target_properties(process-bin PROPERTIES OUTPUT_NAME bin/process)
#target_link_libraries(process-bin process-lib)
#target_compile_options(process-bin PUBLIC "-O2" "-s")

## Debug target
add_executable(process-dbg main.cpp)
set_target_properties(process-dbg PROPERTIES OUTPUT_NAME bin/process-dbg)
target_link_libraries(process-dbg process-lib-dbg)
target_compile_options(process-dbg PUBLIC "-g")

## Testing target
#add_executable(testing-dbg test_main.cpp)
#set_target_properties(testing-dbg PROPERTIES OUTPUT_NAME bin/testing)
#target_link_libraries(testing-dbg process-lib-dbg)
#target_compile_options(testing-dbg PUBLIC "-g")
