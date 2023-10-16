#!/bin/bash


gcc Game_Of_Life_OMP.c -DOUTPUT -fopenmp -o Game_Of_Life_OMP_GIF
gcc Game_Of_Life_OMP.c -fopenmp -o Game_Of_Life_OMP
gcc Game_Of_Life.c -DOUTPUT -o Game_Of_Life_GIF
gcc Game_Of_Life.c -o Game_Of_Life