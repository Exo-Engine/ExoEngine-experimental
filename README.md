# ExoEngine
Designed for Windows. x64 arch only.

**TO-DO**

* IMGUI
* Profiling the engine
* Level Editor
* ARM-64 support
* And more...

## Setup

Install cmake with chocolatey for generating Visual Studio project.   
`choco install cmake.install --installargs '"ADD_CMAKE_TO_PATH=System"'`

You also need https://github.com/microsoft/vcpkg and run the following command:  
`.\vcpkg install glew:x64-windows sdl2:x64-windows sdl2-image:x64-windows imgui:x64-windows openal-soft:x64-windows libvorbis:x64-windows pthreads:x64-windows libxml2:x64-windows glm:x64-windows`

Run the `gen_vs2019_x64.bat` and open the project located at `.build\win64_vs2019`.

