cmake_minimum_required(VERSION 3.8)
project(ExoEngine CXX)

file(GLOB SOURCES
	*.h
	*.cpp
)

link_libraries(ExoEngine)

file(GLOB RESOURCES
	resources/*
)

file(COPY ${RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

add_executable(sound ${SOURCES})

# copy .dll, else LoadLibrary can't load sub-dependency of lib-Exo-* and generate a fatal error
add_custom_command(TARGET sound POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/lib-Exo-Renderer/SDLOpenGL/Debug/ ${CMAKE_CURRENT_BINARY_DIR}
)