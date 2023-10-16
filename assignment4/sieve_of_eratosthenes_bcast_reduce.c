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

    double start = MPI_Wtime();

    int *primes = NULL;
    int primesIndex = 0;

    int *marked = NULL;
    int chunkSize = max / size;
    int k = 0;
    // std::chrono::time_point<std::chrono::system_clock> startTime;
    if (rank == 0)
    {
        // startTime = std::chrono::system_clock::now();
        // Jesus, why malloc...
        marked = (int *)malloc(sizeof(int) * max);
        for (int i = 0; i < max; i++)
        {
            marked[i] = 0;
        }

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
        }
        MPI_Bcast(
            &k,
            1,
            MPI_INT,
            0,
            MPI_COMM_WORLD);

        int *threadMarked = (int *)malloc(sizeof(int) * chunkSize);
        MPI_Scatter(
            marked,
            chunkSize,
            MPI_INT,
            threadMarked,
            chunkSize,
            MPI_INT,
            0,
            MPI_COMM_WORLD);

        mark(k, 0, chunkSize, threadMarked);
        MPI_Gather(
            threadMarked,
            chunkSize,
            MPI_INT,
            marked,
            chunkSize,
            MPI_INT,
            0,
            MPI_COMM_WORLD);

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
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}