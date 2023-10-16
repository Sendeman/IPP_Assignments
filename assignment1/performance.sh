#!/bin/bash

#create performance file
echo "" > performance_exp.txt

#run experiments and append results to file
for T in 1 2 4 8 16 32
do
    for N in 1 2 4 8 16 32
    do 
        echo "RUNNING T=$T, N=$N"
        echo "T=$T, N=$N" >> performance_exp.txt
        ./performance $T $N >> performance_exp.txt
    done
done