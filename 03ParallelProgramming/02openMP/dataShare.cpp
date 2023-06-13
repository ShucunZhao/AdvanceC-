#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string>
#include <iostream>

int main(int argc, char * argv[]){
	/*	
	if(argc!=2){
		printf("Usage:./openMp numOfThreads\n");
		return -1;
	}
	*/
	int myid = 0, numthreads = 1;
	#pragma omp parallel private(myid, numthreads)
	{
	#ifdef _OPENMP
		myid = omp_get_thread_num();
		numthreads = omp_get_num_threads();
	#endif	
	}
	printf("Hello from thread %d of %d in total.\n",myid, numthreads);
	return 0;
}
