# ExoEngine

## For Windows

Install cmake for generating Visual Studio project.   
`choco install cmake.install --installargs '"ADD_CMAKE_TO_PATH=System"'`

You also need https://github.com/microsoft/vcpkg and run the following command:  
`.\vcpkg install glew:x64-windows sdl2:x64-windows sdl2-net:x64-windows sdl2-image:x64-windows openal-soft:x64-windows libvorbis:x64-windows openssl:x64-windows pthreads:x64-windows libxml2:x64-windows glm:x64-windows bullet3:x64-windows`
