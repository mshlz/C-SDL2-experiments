@ECHO OFF
if not exist "..\..\build" mkdir ..\..\build
if not exist "..\..\build\SDL2.dll" copy "..\..\SDL2\lib\SDL2.dll" "..\..\build\SDL2.dll"
if not exist "..\..\build\libpng16-16.dll" copy "..\..\SDL2\bin\libpng16-16.dll" "..\..\build\libpng16-16.dll"
if not exist "..\..\build\SDL2_image.dll" copy "..\..\SDL2\bin\SDL2_image.dll" "..\..\build\SDL2_image.dll"
if not exist "..\..\build\zlib1.dll" copy "..\..\SDL2\bin\zlib1.dll" "..\..\build\zlib1.dll"
copy ".\config.txt" "..\..\build\config.txt"
if not exist "..\..\build\images" mkdir ..\..\build\images
if not exist "..\..\build\images\minesweeper.png" copy ".\images\minesweeper.png" "..\..\build\images\minesweeper.png"

del build/minesweeper.exe

@ECHO ON
gcc -g ../shared/engine.c game.c main.c  -I"../../SDL2/include" -L"../../SDL2/lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -O3  -o ../../build/minesweeper.exe
@REM pause