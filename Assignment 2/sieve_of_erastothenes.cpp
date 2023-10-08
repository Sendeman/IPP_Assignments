#include <iostream>
#include <pthread.h>
#include <math.h>
#include <vector>
#include <chrono>

bool *marked;
std::vector<int> primes;

void sequentialSieve(int max, int primeLimit)
{
    int k = 2;
    while (k * k < primeLimit)
    {
        for (int i = k * 2; i < max; i += k)
        {
            marked[i] = true;
        }

        for (int i = k + 1; i < max; i++)
        {
            if (!marked[i])
            {
                primes.push_back(i);
                k = i;
                break;
            }
        }
    }
}

struct MarkArgs
{
    int k;
    int rangeStart;
    int rangeEnd;
    MarkArgs(int k, int rangeStart, int rangeEnd) : k(k),
                                                    rangeStart(rangeStart),
                                                    rangeEnd(rangeEnd) {}
};

void *mark(void *args)
{
    MarkArgs *myArgs = (MarkArgs *)args;
    for (int i = myArgs->rangeStart; i < myArgs->rangeEnd; i++)
    {
        if (i % myArgs->k == 0)
        {
            marked[i] = true;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int max = std::stoi(argv[1]);
    int threadCount = std::stoi(argv[2]);
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    primes.push_back(2);

    marked = new bool[max];
    sequentialSieve(max, sqrt(max));

    int chunckSize = (max - sqrt(max) + 1) / threadCount;
    pthread_t threads[threadCount];

    int k = primes.back();
    int start = sqrt(max) + 1;
    while (k * k < max)
    {
        for (int i = 0; i < threadCount; i++)
        {
            MarkArgs args(k, start + i * chunckSize, start + (i + 1) * chunckSize);
            pthread_create(&threads[i], NULL, mark, &args);
        }

        for (int i = 0; i < threadCount; i++)
        {
            pthread_join(threads[i], NULL);
        }

        for (int i = k + 1; i < max; i++)
        {
            if (!marked[i])
            {
                primes.push_back(i);
                k = i;
                break;
            }
        }
    }

    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();

    for (auto prime : primes)
    {
        std::cout << prime << ", ";
    }

    std::cout << std::endl
              << "Executed in: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms with " << threadCount << " threads" << std::endl;

    return 0;
}