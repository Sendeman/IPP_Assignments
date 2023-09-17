#!/bin/bash

#create performance file
echo "AUC, Thread time, Sum time" > performance_exp.txt

#run experiments and append results to file
for T in 1 5 10 15 20 25
do
    for N in 1 20 50 200 500 2000 5000 20000 50000 100000 200000 500000
    do 
        echo "RUNNING N=$N, T=$T"
        # echo "n=$N, t=$T" >> performance_exp.txt
        ./numerical_integration $N $T >> performance_exp.txt
    done
done