#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <numeric>

void usage(char *program)
{
  std::cout << "Usage: " << program << " N  (where 2<=N<=10)" << std::endl;
  exit(1);
}

float func(float x)
{
  return 4 / (1 + pow(x, 2));
}

void trapArea(int i, float start, float stepsize, float *result, bool *flag)
{
  float a = start + i * stepsize;
  float b = start + (i + 1) * stepsize;
  *result = (func(a) + func(b)) * ((b - a) / 2);
  *flag = false;
}

int main(int argc, char *argv[])
{
  int n = 5;
  int threadCount = 2;
  bool flags[threadCount];
  std::thread *threads = new std::thread[threadCount];

  float startx = 0;
  float stopx = 1;
  float stepsize = (stopx - startx) / n;

  float values[n];

  bool assigned;
  for (int i = 0; i < n; i++)
  {
    while (!assigned) // while the index is not assigned
    {
      for (int t = 0; t < threadCount; t++) // loop over all threads
      {
        if (!flags[t]) // assign if available
        {
          flags[t] = true;
          threads[t] = std::thread(&trapArea, i, startx, stepsize, &values[i], &flags[t]);
          assigned = true;

          std::cout << "index " << i << "assigned to thread "<< t << std::endl;

        }
      }
    }

    // std::cout << a << " " << b << " " << values[i] << std::endl;
  }

  for (int t = 0; t < threadCount; t++) // wait for threads to finish
  {
    threads[t].join();
  }

  float totalArea = 0;
  totalArea = std::accumulate(values, values + n, totalArea);
  std::cout << totalArea << std::endl;
}