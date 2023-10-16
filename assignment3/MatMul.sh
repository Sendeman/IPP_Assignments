#!/bin/bash

#create performance file
echo "" > MatMul.txt

# #run experiments and append results to file

for threadnum in 1 2 4 8 16;
do
    echo "NUMTHREADS=$threadnum"
    export OMP_NUM_THREADS=$threadnum
    # echo "n=$N, t=$T" >> performance_exp.txt
    echo "OMP_NUM_THREADS=$threadnum" >> MatMul.txt
    ./MatMul >> MatMul.txt
done



#run experiments and append results to file
# for N in 1024
# do
#     for T in 1000
#     do
#         echo "RUNNING N=$N, T=$T"
#         # echo "n=$N, t=$T" >> performance_exp.txt
#         ./Game_Of_Life $N $T >> Game_Of_Life.txt
#         ./Game_Of_Life_OMP $N $T >> Game_Of_Life.txt
#     done
# done