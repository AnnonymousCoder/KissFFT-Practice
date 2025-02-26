main: main.c
	gcc main.c -o main.exe -O3 -std=c99 -Wall -Wno-missing-braces -I include -L lib -l:libkfft-float.a -lraylib -lopengl32 -lgdi32 -lwinmm -fopenmp

debug: main.c
	gcc main.c -g -o main.exe -O3 -std=c99 -Wall -Wno-missing-braces -I include -L lib -l:libkfft-float.a -lraylib -lopengl32 -lgdi32 -lwinmm -fopenmp