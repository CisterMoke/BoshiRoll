cmake_minimum_required(VERSION 3.20)
project(BoshiRoll VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

set(SDL2_PATH "${PROJECT_SOURCE_DIR}/../Libs/SDL2-2.0.16" CACHE PATH "" FORCE)
set(SDL2_IMAGE_PATH "${PROJECT_SOURCE_DIR}/../Libs/SDL2_image-2.0.5" CACHE PATH "" FORCE)
set(SDL2_GFX_PATH "${PROJECT_SOURCE_DIR}/../Libs/SDL2_gfx" CACHE PATH "" FORCE)
set(SDL2_TTF_PATH "${PROJECT_SOURCE_DIR}/../Libs/SDL2_ttf-2.0.15" CACHE PATH "" FORCE)

set(SRC_DIR "src")
set(REC_DIR "resources")
set(LIB_DIR "lib")


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_DIR}/main.cpp)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
	${SDL2_GFX_INCLUDE_DIRS})

add_subdirectory(${SRC_DIR})

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		SDL2::Main
		SDL2::Image SDL2::GFX
		SDL2::TTF
		algebra
		sprites
		utils
		colliders
		entities
		level
		)

add_custom_command(TARGET BoshiRoll POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/${LIB_DIR} $<TARGET_FILE_DIR:BoshiRoll>
	)

if ($<CONFIG:x64-Debug,x86-Debug,Debug>)
	set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
else()
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/${REC_DIR} $<TARGET_FILE_DIR:BoshiRoll>/${REC_DIR}
	)
endif()