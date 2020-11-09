cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

file(
	GLOB_RECURSE source_list RELATIVE
	"${CMAKE_CURRENT_SOURCE_DIR}"
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

include_directories(include)

link_libraries(ExoEngine)

file(GLOB RESOURCES
	resources/*
)

file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_executable(Editor ${source_list})
