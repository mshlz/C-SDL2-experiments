@ECHO OFF
if not exist "build" mkdir build
if not exist "build\SDL2.dll" copy "SDL2\lib\SDL2.dll" "build\SDL2.dll"

del build\*.exe

@ECHO ON
gcc -g src/engine.c src/game.c src/main.c  -I"./SDL2/include" -L"./SDL2/lib" -lmingw32 -lSDL2main -lSDL2 -O3 -Wall -o build/game.exe
pause