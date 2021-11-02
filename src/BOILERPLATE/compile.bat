@ECHO OFF
if not exist "..\..\build" mkdir "..\..\build"
if not exist "..\..\build\SDL2.dll" copy "..\..\SDL2\lib\SDL2.dll" "..\..\build\SDL2.dll"

del build\*.exe

@ECHO ON
@REM to hide console add -mwindows flag to compiler
gcc -g ../shared/engine.c game.c main.c  -I"./SDL2/include" -L"./SDL2/lib" -lmingw32 -lSDL2main -lSDL2 -O3 -Wall -o ../../build/game.exe
pause