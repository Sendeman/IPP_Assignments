#include <omp.h>
#include <cstdlib>
#include <sys/time.h>
#include <stdio.h>
#include <iostream>

// int a[dim][dim];
// int b[dim][dim];
// int c[dim][dim];


int main(int argc, char *argv[])
{
    const int dim = 3000;
    int **a = new int *[dim];
    for (int i = 0; i < dim; i++)
    {
        a[i] = new int[dim];
    }

    int **b = new int *[dim];
    for (int i = 0; i < dim; i++)
    {
        b[i] = new int[dim];
    }

    int **c = new int *[dim];
    for (int i = 0; i < dim; i++)
    {
        c[i] = new int[dim];
    }

    double time; // variables for timing
    struct timeval ts, tf;

    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim; j++)
        {
            a[i][j] = (std::rand() % 100);
            b[i][j] = (std::rand() % 100);
        }

    // TEST FOR COLLAPSE 1
    c[0][0] = {0};
    gettimeofday(&ts, NULL);
#pragma omp parallel default(none) shared(a, b, c, dim)
#pragma omp for schedule(static) collapse(1)
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    gettimeofday(&tf, NULL);
    time = (tf.tv_sec - ts.tv_sec) + (tf.tv_usec - ts.tv_usec) * 0.000001;
    printf("Collapsenum=1, time=%lf\n", time);

    // TEST FOR COLLAPSE 2
    c[0][0] = {0};
    gettimeofday(&ts, NULL);
#pragma omp parallel default(none) shared(a, b, c, dim)
#pragma omp for schedule(static) collapse(2)
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    gettimeofday(&tf, NULL);
    time = (tf.tv_sec - ts.tv_sec) + (tf.tv_usec - ts.tv_usec) * 0.000001;
    printf("Collapsenum=2, time=%lf\n", time);

    // TEST FOR COLLAPSE 3
    c[0][0] = {0};
    gettimeofday(&ts, NULL);
#pragma omp parallel default(none) shared(a, b, c, dim)
#pragma omp for schedule(static) collapse(3)
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    gettimeofday(&tf, NULL);
    time = (tf.tv_sec - ts.tv_sec) + (tf.tv_usec - ts.tv_usec) * 0.000001;
    printf("Collapsenum=3, time=%lf\n", time);
}