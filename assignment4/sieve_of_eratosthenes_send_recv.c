#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

void printArray(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

void sequentialSieve(int max, int primeLimit, int marked[], int primes[], int *primesIndex)
{
    int k = 2;
    while (k * k < primeLimit)
    {
        for (int i = k * 2; i < max; i += k)
        {
            marked[i] = 1;
        }

        for (int i = k + 1; i < max; i++)
        {
            if (!marked[i])
            {
                primes[*primesIndex] = i;
                k = i;
                (*primesIndex)++;
                break;
            }
        }
    }
}

void mark(int k, int rangeStart, int rangeEnd, int *marked)
{
    for (int i = rangeStart; i < rangeEnd; i++)
    {
        if (i % k == 0)
        {
            marked[i] = 1;
        }
    }
}

int main(int argc, char **argv)
{
    /* no MPI calls before this point */
    int max = atoi(argv[1]);
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    enum { MARKED_TAG, K_TAG };
    double start = MPI_Wtime();

    int *primes = NULL;
    int primesIndex = 0;

    int *marked = NULL;
    int chunkSize = max / size;
    int k = 0;
    int markedvals[max/2];
    // std::chrono::time_point<std::chrono::system_clock> startTime;
    if (rank == 0)
    {
        // startTime = std::chrono::system_clock::now();
        // Init marked array of 0
        marked = (int *)malloc(sizeof(int) * max);
        for (int i = 0; i < max; i++)
        {
            marked[i] = 0;
        }

        // init primes array of 0
        primes = (int *)malloc(sizeof(int) * max);
        for (int i = 0; i < max; i++)
        {
            primes[i] = 0;
        }

        primes[0] = 2;
        primesIndex = 1;
        sequentialSieve(max, sqrt(max), marked, primes, &primesIndex);
    }

    while (k * k < max)
    {
        if (rank == 0)
        {
            k = primes[primesIndex - 1];    

            //send k and marked array for computation
            for(int r=1; r<size;r++){
                MPI_Send(&k, 1, MPI_INT, 0, K_TAG, MPI_COMM_WORLD);  
                // MPI_Send(, 1, MPI_INT, 0, K_TAG, MPI_COMM_WORLD);  
            }

            //receive all marked integers
            for (int r=1; r<size; r++){
                MPI_Recv(&markedvals, size/2, MPI_INT, r, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //extract marked vals
                for (int i=0; i<size/2; i++){
                    if(markedvals[i]==0){
                        //no more markedvalues
                        break;
                    }
                    marked[markedvals[i]]=1;
                }
            }
        } else { //so for non-main threads 
            // receive k for computation?
            MPI_Recv(&k, 1, MPI_INT, 0, K_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // receive marked array for editing
            // MPI_Recv(&marked, sizeof(marked), MPI_INT, 0, MARKED_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)

            //init (partial)marked array for k
            // int markedvals[max/2]; //max/2 is the maximum nr of marked values i think
            for(int i=0; i<(max/2);i++){
                markedvals[0] = 0;
            }

            int addindex = 0;
            //for each rank have own product values
            for (int p=rank+2; p<k; p+=size){
                //mark multiple of k
                markedvals[addindex] = p*k;
                addindex++;
            }

            //return (partial)marked array
            MPI_Send(&markedvals, max/2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        
        // find the next non-marked number after our prime, save it and break
        if (rank == 0)
        {
            for (int i = k + 1; i < max; i++)
            {
                if (!marked[i])
                {
                    primes[primesIndex] = i;
                    primesIndex++;
                    break;
                }
            }
        }
    }

    if (rank == 0)
    {
        double stop = MPI_Wtime();
        // printArray(primes, primesIndex);
        printf("Executed in %lf seconds with %d threads\n", stop - start, size);
        for (int i=0; i<max;i++){
            if (primes[i]!=0){
                printf("%d", primes[i]);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}