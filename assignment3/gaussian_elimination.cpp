#include <iostream>
#include <chrono>
#include <omp.h>
#include <random>

using namespace std;

struct Matrix
{
    int **leftSide;
    int *rightSide;
    int _n;

    void generateData()
    {
        #pragma omp parallel for default(shared) collapse(2)
        for (int i = 0; i < _n; i++)
        {
            for (int j = i; j < _n; j++)
            {
                if(i == j) rightSide[i] = i + 1;
                leftSide[i][j] = i + j + 1;
            }
        }
    }

    void printData()
    {
        cout << "Right side:" << endl;
        for (int i = 0; i < _n; i++)
        {
            cout << rightSide[i] << ","
                 << "\t";
        }
        cout << endl
             << "Left side: " << endl;
        for (int i = 0; i < _n; i++)
        {
            for (int j = 0; j < _n; j++)
            {
                cout << leftSide[i][j] << ","
                     << "\t";
            }
            cout << endl;
        }
    }

    Matrix(int n)
    {
        _n = n;
        rightSide = new int[n]{0};
        leftSide = new int *[n];
        for (int i = 0; i < n; i++)
        {
            leftSide[i] = new int[n]{0};
        }
        generateData();
        // printData();
    }
};

int main(int argc, char *argv[])
{
    int n = std::stoi(argv[1]);
    cout << "Generating linear system... ";
    Matrix matrix(n);
    cout << "Done";
    int **A = matrix.leftSide;
    int *b = matrix.rightSide;
    double x[n] = {0}; // answer

    chrono::time_point<chrono::system_clock> startTime = chrono::system_clock::now();

    // https://stackoverflow.com/questions/48027598/why-this-openmp-parallel-for-loop-doesnt-work-properly

    for (int row = n - 1; row >= 0; row--)
        {
            double acc = b[row];
        #pragma omp parallel for schedule(runtime) firstprivate(row, n, A) reduction(- : acc)
            for (int col = row + 1; col < n; col++)
                acc -= A[row][col] * x[col];
            x[row] = acc / A[row][row];
        }

    chrono::time_point<chrono::system_clock> endTime = chrono::system_clock::now();

    // cout << endl;
    // for (int i = 0; i < n; i++)
    // {
    //     cout << "x" << i << " = " << x[i] << endl;
    // }

    cout << endl
         << "System with " << n << " equations solved in: " << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count() << " ms" << endl;
}