#!/bin/bash

#create performance file
echo "" > Game_Of_Life.txt

# #run experiments and append results to file
for N in 64 1024 4096
do
    for T in 1000 2000
    do
        for threadnum in 1 2 4 8 16;
        do
            echo "RUNNING N=$N, T=$T, NUMTHREADS=$threadnum"
            export OMP_NUM_THREADS=$threadnum
            # echo "n=$N, t=$T" >> performance_exp.txt
            echo "OMP_NUM_THREADS=$threadnum" >> Game_Of_Life.txt
            ./Game_Of_Life $N $T >> Game_Of_Life.txt
            ./Game_Of_Life_OMP $N $T >> Game_Of_Life.txt
        done
    done
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