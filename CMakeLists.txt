cmake_minimum_required(VERSION 3.8)

if (WIN32)
	set(CMAKE_TOOLCHAIN_FILE C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif ()

project(ExoEngine CXX)

set(CMAKE_CXX_STANDARD 17)
subdirs(examples)

file(
	GLOB_RECURSE source_list RELATIVE
	"${CMAKE_CURRENT_SOURCE_DIR}"
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

include_directories(
	include
	${LIBXML2_INCLUDE_DIR})
	
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Threads REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "/W4 /FS /EHsc")
else ()
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

link_libraries(SDL2::SDL2 SDL2::SDL2_image ${CMAKE_THREAD_LIBS_INIT} ${LIBXML2_LIBRARIES} OpenGL::GL GLEW::GLEW glm OpenAL::OpenAL Vorbis::vorbis Vorbis::vorbisenc Vorbis::vorbisfile)
add_library(ExoEngine STATIC ${source_list})

