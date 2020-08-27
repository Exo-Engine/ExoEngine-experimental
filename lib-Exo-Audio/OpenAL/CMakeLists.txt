cmake_minimum_required(VERSION 3.8)
project(ExoAudioOpenAL CXX)

set(AUDIO_DIR ../audio)

file(GLOB SOURCES
	${AUDIO_DIR}/include/*.h
	${AUDIO_DIR}/include/**/**.h
	${AUDIO_DIR}/src/*.cpp
	${AUDIO_DIR}/src/**/**.cpp
	include/*.h
	include/**/**.h
	src/*.cpp
	src/**/**.cpp
)

include_directories(
	include
	${AUDIO_DIR}/include)

link_libraries(openal vorbisfile)

add_library(ExoAudioOpenAL SHARED ${SOURCES})
