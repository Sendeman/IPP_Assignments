#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

void usage(char *program)
{
  std::cout << "Usage: " << program << " N  (where 2<=N<=10)" << std::endl;
  exit(1);
}

float func(float x){
  return 4/(1+pow(x, 2));
}

float trapArea(float a, float b){
  return (func(a) + func(b))*((b-a)/2);
}

int main(int argc, char *argv[])
{
  float startx = 0;
  float stopx = 1;
  float stepsize = .05;

  float a=startx;
  float b=startx+stepsize;
  float totalArea = 0;

  while(b<stopx+stepsize){
    totalArea+=trapArea(a,b);
      std::cout << a << " " << b << " " << trapArea(a,b) << std::endl;

    a+=stepsize;
    b+=stepsize;
  }

  std::cout << totalArea << std::endl;

  // if (argc != 2)
  // {
  //   usage(argv[0]);
  // }


}