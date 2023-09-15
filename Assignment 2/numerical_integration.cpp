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

void trapArea(int i, float start, float stepsize, float *result)
{
  float a = start + i * stepsize;
  float b = start + (i + 1) * stepsize;
  *result = (func(a) + func(b)) * ((b - a) / 2);
}

int main(int argc, char *argv[])
{
  int n = 20;
  int threadCount = 5;
  
  float startx = 0;
  float stopx = 1;
  float stepsize = (stopx - startx) / n;

  float values[n];
  std::thread *ta = new std::thread[threadCount];
  // bool flags[threadCount];

  for (int i = 0; i < n; i++)
  {
    ta[i] = std::thread(&trapArea, i, startx, stepsize, &values[i]);
    // std::cout << a << " " << b << " " << values[i] << std::endl;
  }

  for(int i = 0; i < n; i++)
  {
    ta[i].join();
  }

  float totalArea = 0;
  totalArea = std::accumulate(values, values + n, totalArea);
  std::cout << totalArea << std::endl;
}