cmake_minimum_required(VERSION 3.9.0)

project(sdl-weekend)

option(SDL3 "Use SDL3 instead of SDL2" OFF)
option(STANDALONE "Build without framework" OFF)

#set(SANITIZE "-fsanitize=address")
set(CMAKE_C_FLAGS_DEBUG "-pipe -DDEBUG -O3 -g -ggdb ${SANITIZE}")
set(CMAKE_CXX_FLAGS_DEBUG "-pipe -DDEBUG -O3 -g -ggdb ${SANITIZE}")
set(CMAKE_C_FLAGS_RELEASE "-pipe -DNDEBUG -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-pipe -DNDEBUG -O3")

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
	set(WARNINGS
		-Wall
		-Wextra
		-Wfloat-conversion)
	set(NO_WARNINGS
		-Wno-missing-braces)
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	set(WARNINGS
		-Wall
		-Wextra
		-Wfloat-conversion)
	set(NO_WARNINGS
		-Wno-missing-braces)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	set(WARNINGS
		/W4)
	set(NO_WARNINGS
		/D_CRT_SECURE_NO_WARNINGS)
endif()

add_compile_options(
	${WARNINGS}
	${NO_WARNINGS})

if (CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
	add_compile_options(-DSDL_DISABLE_IMMINTRIN_H)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(copy-compile-commands ALL
	${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_BINARY_DIR}/compile_commands.json
	${CMAKE_CURRENT_LIST_DIR})

if (NOT CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
	find_package(OpenMP REQUIRED)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(SDL3)
	set(SDL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/SDL3/include
			${CMAKE_CURRENT_SOURCE_DIR}/SDL3/include/SDL3)
	set(SDL_LINK_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/SDL3/bin
			${CMAKE_CURRENT_SOURCE_DIR}/SDL3/lib)
	set(SDL_LIBRARIES SDL3)
else()
	find_package(SDL2 REQUIRED)
	set(SDL_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
	set(SDL_LINK_DIRS)
	set(SDL_LIBRARIES ${SDL2_LIBRARIES})
endif()

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

if (NOT STANDALONE STREQUAL "Yes")
set(FRAMEWORK_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/framework/foundation/include
	${CMAKE_CURRENT_SOURCE_DIR}/framework/soa_components/include
	${CMAKE_CURRENT_SOURCE_DIR}/framework/soa_entities/include
	${CMAKE_CURRENT_SOURCE_DIR}/framework/soa_systems/include)
file(GLOB_RECURSE FRAMEWORK_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/framework/foundation/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/framework/soa_components/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/framework/soa_entities/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/framework/soa_systems/src/*.c)
endif()
file(GLOB_RECURSE GAME_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/${GAME}/*.c)

add_executable(${GAME}
	${FRAMEWORK_SRC}
	${GAME_SRC})
target_include_directories(${GAME}
	PRIVATE ${SDL_INCLUDE_DIRS}
	PRIVATE ${FRAMEWORK_INCLUDE_DIRS})
target_link_directories(${GAME}
	PRIVATE ${SDL_LINK_DIRS})
target_link_libraries(${GAME}
	PRIVATE ${SDL_LIBRARIES}
	PRIVATE m)

if (NOT STANDALONE STREQUAL "Yes")
add_executable(${GAME}_test
	${CMAKE_CURRENT_SOURCE_DIR}/test/main.c)
target_include_directories(${GAME}_test
	PRIVATE ${FRAMEWORK_INCLUDE_DIRS})
add_test(NAME ${GAME}_test
	COMMAND ${GAME}_test)
endif()

if(NOT DEFINED OUTPUT)
	set(OUTPUT "bin")
endif()
set(OUTPUT_PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY	 ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}
	LIBRARY_OUTPUT_DIRECTORY	 ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}
	LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT})

set_target_properties(${GAME} PROPERTIES ${OUTPUT_PROPERTIES})
if (NOT STANDALONE STREQUAL "Yes")
set_target_properties(${GAME}_test PROPERTIES ${OUTPUT_PROPERTIES})
endif()
