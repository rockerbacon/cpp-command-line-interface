cmake_minimum_required(VERSION 3.10)

project("assertions" CXX)
enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_STATIC_LIBRARY ON)
set(BUILD_SHARED_LIBRARY OFF)

set(tests_src_dir "tests")
set(objs_src_dir "src/objs")
set(programs_src_dir "src/main")

set(warnings "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${warnings}")

include_directories(${objs_src_dir})

file(GLOB_RECURSE objs_src_files "${objs_src_dir}/*.cpp")
foreach(obj_src_file  ${objs_src_files})
	string(REGEX REPLACE "(^(.*/)*${objs_src_dir}/(.*/)*)|(\.cpp)" "" obj_binary ${obj_src_file})
	add_library(${obj_binary} OBJECT ${obj_src_file})
	list(APPEND all_obj_binaries $<TARGET_OBJECTS:${obj_binary}>)
endforeach()

file(GLOB_RECURSE tests_src_files "${tests_src_dir}/*.cpp")
foreach(test_src_file ${tests_src_files})
	string(REGEX REPLACE "(^(.*/)*${tests_src_dir}/)|(.cpp)" "" test_binary ${test_src_file})
	add_executable(${test_binary} ${test_src_file} ${all_obj_binaries})
	add_test(NAME ${test_binary} COMMAND ${test_binary})
endforeach()	

file(GLOB programs_src_files "${programs_src_dir}/*.cpp")
foreach(program_src_file ${programs_src_files})
	string(REGEX REPLACE "(^(.*/)*${programs_src_dir}/)|(.cpp)" "" program_binary ${program_src_file})
	add_executable(${program_binary} ${program_src_file} ${all_obj_binaries})
endforeach()

if (${BUILD_STATIC_LIBRARY})
	add_library(${PROJECT_NAME} STATIC ${all_obj_binaries})
endif()
if (${BUILD_SHARED_LIBRARY})
	add_library(${PROJECT_NAME} SHARED ${all_obj_binaries})
endif()

