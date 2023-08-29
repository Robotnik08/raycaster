del build\main.exe
taskkill /IM main.exe /F
gcc -I src/include -L src/lib -o build/main main.c -lmingw32 -lSDL2main -lSDL2
start build/main.exe