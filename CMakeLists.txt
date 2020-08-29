cmake_minimum_required(VERSION 3.8)

if (WIN32)
	set(CMAKE_TOOLCHAIN_FILE C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(ExoEngine CXX)

set(CMAKE_CXX_STANDARD 17)
subdirs(examples)

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
	${LIBXML2_INCLUDE_DIR}
	lib-Exo-Renderer/renderer/include
	lib-Exo-Audio/audio/include)
	
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-net CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Threads REQUIRED)
find_package(Bullet CONFIG REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "/W4 /FS /EHsc")
else ()
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif ()

link_libraries(SDL2::SDL2 SDL2::SDL2_net OpenSSL::SSL OpenSSL::Crypto ${CMAKE_THREAD_LIBS_INIT} ${LIBXML2_LIBRARIES} glm  LinearMath Bullet3Common BulletDynamics BulletSoftBody)
add_library(ExoEngine STATIC ${SOURCES})
