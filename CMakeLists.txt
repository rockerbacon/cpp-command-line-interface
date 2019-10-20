cmake_minimum_required(VERSION 3.10)

project("cpp_cli" CXX)
enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_STATIC_LIBRARY ON)
set(BUILD_SHARED_LIBRARY OFF)

set(tests_src_dir "tests")
set(objs_src_dir "src/objs")
set(programs_src_dir "src/main")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

	set(external_libraries "pthread")
	set(compiler_flags "-fdiagnostics-color=always -Wall -Wextra")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

	set(external_libraries "")
    set(compiler_flags "/W4 /EHsc")

endif()

set(CMAKE_CXX_FLAGS "${compiler_flags}")

include_directories(${objs_src_dir})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY release/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY release/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY release/bin)

file(GLOB_RECURSE objs_src_files "${objs_src_dir}/*.cpp")
foreach(obj_src_file  ${objs_src_files})
	string(REGEX REPLACE "(^(.*/)*${objs_src_dir}/(.*/)*)|(\.cpp)" "" obj_binary ${obj_src_file})
	add_library(${obj_binary} OBJECT ${obj_src_file})
	list(APPEND all_obj_binaries $<TARGET_OBJECTS:${obj_binary}>)
endforeach()

file(GLOB_RECURSE tests_src_files "${tests_src_dir}/*.cpp")
foreach(test_src_file ${tests_src_files})
	string(REGEX REPLACE "(^(.*/)*${tests_src_dir}/)|(.cpp)" "" test_binary ${test_src_file})
	string(REGEX REPLACE "/" "_" test_binary ${test_binary})
	string(PREPEND test_binary "tests_")
	add_executable(${test_binary} ${test_src_file} ${all_obj_binaries})
	set_target_properties(${test_binary}
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "tests"
	)
	target_link_libraries(${test_binary} ${external_libraries})
	add_test(NAME ${test_binary} COMMAND ${test_binary})
endforeach()

file(GLOB_RECURSE programs_src_files "${programs_src_dir}/*.cpp")
foreach(program_src_file ${programs_src_files})
	string(REGEX REPLACE "(^(.*/)*${programs_src_dir}/)|(.cpp)" "" program_binary ${program_src_file})
	string(REGEX REPLACE "/" "_" program_binary ${program_binary})
	add_executable(${program_binary} ${program_src_file} ${all_obj_binaries})
	target_link_libraries(${program_binary} ${external_libraries})
endforeach()

if (${BUILD_STATIC_LIBRARY})
	add_library(${PROJECT_NAME} STATIC ${all_obj_binaries})
endif()
if (${BUILD_SHARED_LIBRARY})
	add_library(${PROJECT_NAME} SHARED ${all_obj_binaries})
endif()

if (${BUILD_STATIC_LIBRARY} OR ${BUILD_SHARED_LIBRARY})
	file (
		COPY ${objs_src_dir}/
		DESTINATION release/include
		FILES_MATCHING PATTERN "*.h"
	)
endif()

