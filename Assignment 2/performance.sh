#!/bin/bash

#create performance file
echo "Params,AUC,Thread time,Sum time" > performance_exp.txt

#run experiments and append results to file
for N in 1 20 200 2000 20000 200000 500000
do
    for T in 1 5 15 30
    do 
        echo "RUNNING N=$N, T=$T"
        # echo "n=$N, t=$T" >> performance_exp.txt
        ./numerical_integration $N $T >> performance_exp.txt
    done
done