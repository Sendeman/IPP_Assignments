#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <numeric>
#include <chrono>

std::mutex ilock;

void usage(char *program)
{
  std::cout << "Usage: " << program << " [number_of_trapezoids] [number_of_threads] " << std::endl;
  exit(1);
}

float func(float x)
{
  return 4 / (1 + pow(x, 2));
}

void trapArea(int *i, float start, float stepsize, float *result, std::mutex *lock)
{
  ilock.lock();
  int myi = *i;
  *i++;
  ilock.unlock();

  float a = start + myi * stepsize;
  float b = start + (myi + 1) * stepsize;
  *result = (func(a) + func(b)) * ((b - a) / 2);
  // std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main(int argc, char *argv[])
{
  if (argc == 2 && (std::string(argv[1]) == "-h"))
  {
    usage(argv[0]);
  }

  int n = std::stoi(argv[1]);
  int threadCount = std::stoi(argv[2]);
  std::thread *threads = new std::thread[threadCount];

  float startx = 0;
  float stopx = 1;
  float stepsize = (stopx - startx) / n;

  float values[n];

  int i = 0;
  auto start_time = std::chrono::system_clock::now();
  bool firstRun = true;
  
  while (i < n)
  {
    for (int t = 0; t < threadCount; t++) // loop over all threads
    {
      if (firstRun)
      {
        threads[t] = std::thread(&trapArea, &i, startx, stepsize, &values[i], &ilock);
      }
      else if (threads[t].joinable())
      { // #https://stackoverflow.com/questions/42692538/c-thread-still-joinable-after-it-finishes-execution
        threads[t].join();
        threads[t] = std::thread(&trapArea, &i, startx, stepsize, &values[i], &ilock);
      }
      // std::cout << "index " << i << " assigned to thread " << t << std::endl;
    }
    firstRun = false;
  }

  for (int t = 0; t < threadCount; t++) // wait for threads to finish
  {
    threads[t].join();
  }
  auto break_point = std::chrono::system_clock::now();

  float totalArea = 0;
  totalArea = std::accumulate(values, values + n, totalArea);
  auto end_time = std::chrono::system_clock::now();

  std::cout << ',' << "N=" << n << " T=" << threadCount << ',' << totalArea << ',' << (break_point - start_time).count() << ',' << (end_time - break_point).count() << std::endl;
  // std::cout << "AUC: " << totalArea << std::endl;
  // std::cout << "thread time " << (break_point - start_time).count() << " sum time " << (end_time - break_point).count() << std::endl;
}