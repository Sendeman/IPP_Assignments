#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

    int main(int argc, char *argv[])
{
    int N = 12;
    int i, j, ioffs, joffs;

	for (ioffs=1; ioffs<4; ioffs++)
	for	(joffs=1; joffs<4; joffs++){
		printf("ioffset:%d  joffset:%d\n----------------------\n", ioffs, joffs);
		#pragma omp parallel for collapse(2)
			for (i = ioffs ; i < N-1 ; i+=3){
				for (j = joffs ; j < N-1 ; j+=3) {	
					printf("i:%d j:%d\n", i, j);
				}
			}
		

		printf("\n\n");
		}
}