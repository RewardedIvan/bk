@echo off
mkdir .temp
cd .temp
mkdir libs

echo ============ downloading glew.zip...
curl -LJO https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip

echo ============ extracting glew.zip
"%PROGRAMFILES%\7-Zip\7z.exe" x glew-2.2.0.zip
move glew-2.2.0 libs\glew

echo ============ downloading glfw for windows...
curl -LJO https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN64.zip
curl -LJO https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN32.zip

echo ============ extracting glfw for windows...
"%PROGRAMFILES%\7-Zip\7z.exe" x glfw-3.3.9.bin.WIN64.zip
"%PROGRAMFILES%\7-Zip\7z.exe" x glfw-3.3.9.bin.WIN32.zip

echo ============ organizing glfw libs...
mkdir libs\glfw3\lib\
move /Y glfw-3.3.9.bin.WIN64\lib-vc2022 libs\glfw3\lib\x64
move /Y glfw-3.3.9.bin.WIN32\lib-vc2022 libs\glfw3\lib\win32

move /Y glfw-3.3.9.bin.WIN32\include libs\glfw3\
move /Y glfw-3.3.9.bin.WIN32\LICENSE.md libs\glfw3\
move /Y glfw-3.3.9.bin.WIN32\README.md libs\glfw3\

rem TODO: turn these into submodules?

echo ============ cloning glm...
git clone https://github.com/g-truc/glm.git libs\glm

echo ============ cloning stb...
git clone https://github.com/nothings/stb libs\stb

echo ============ cleaning up stb...
mkdir libs\stb\other
for /d %%i in ("libs\stb\*") do (
    if /i not "%%i"=="libs\stb\.git" (
		if /i not "%%i"=="libs\stb\other" (
			echo %%i -> libs\stb\other\
			move "%%i" "libs\stb\other\"
		)
    )
)

echo ============ cloning imgui...
git clone https://github.com/ocornut/imgui libs\imgui

echo ============ adding a visual studio project to imgui...
copy /Y ..\resources\imgui.* libs\imgui\

echo ============ enabling freetype in imconfig...
echo. >> libs\imgui\imconfig.h
echo #define IMGUI_ENABLE_FREETYPE >> libs\imgui\imconfig.h

echo ============ cloning imgui_club...
git clone https://github.com/ocornut/imgui_club libs\imgui_club

echo ============ cloning imgui-filebrowser...
git clone https://github.com/AirGuanZ/imgui-filebrowser libs\imgui-filebrowser

echo ============ cloning freetype...
git clone https://gitlab.freedesktop.org/freetype/freetype libs\freetype

echo ============ cleaning up...
move libs ..\..\libs
cd ..
rmdir /S /Q .temp