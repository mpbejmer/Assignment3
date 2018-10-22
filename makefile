all : cell_distance

cell_distance : cell_distance.c
	gcc -O2 -march=native -o cell_distance cell_distance.c -lm -fopenmp
