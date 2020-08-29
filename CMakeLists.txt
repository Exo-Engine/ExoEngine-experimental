cmake_minimum_required(VERSION 3.8)

if (WIN32)
	set(CMAKE_TOOLCHAIN_FILE C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif

project(ExoEngine CXX)

subdirs(examples)

add_subdirectory(third_party/bullet3)
add_subdirectory(lib-Exo-Renderer)
add_subdirectory(lib-Exo-Audio)

file(GLOB SOURCES
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

include_directories(
	include
	third_party/bullet3/src
	lib-Exo-Renderer/renderer/include
	lib-Exo-Audio/audio/include)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "/W4 /FS /EHsc")
else ()
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif ()

link_libraries(SDL2_net ssl crypto dl pthread xml2 BulletDynamics BulletCollision LinearMath)

add_library(ExoEngine STATIC ${SOURCES})
