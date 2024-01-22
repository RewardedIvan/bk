# bk

bk is a low level game engine/framework written in c++

## dependencies

- imgui (optional)
- glfw & glew (optional)
- glm
- stb

## build

### windows

> (you need to have 7-zip, curl and git installed for the script to run)  
1. run `FetchLibs.bat` in the `scripts` directory  
2. open the solution in visual studio 2023  
3. right click on the `glew_static` project and click `Retarget Projects`, and `OK`  
4. then collapse all projects except `bk`, click on `bk` and the `Show All files` button

### unix

todo

## todo
- support unix
- add vulkan
- build system?
- move libs to submodules
- mip maps
