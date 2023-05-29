#include <mpi.h>//You need to run the command line "yum install openmpi openmpi-devel" for get the library.
#include<iostream>

/*
   After install te mpi-devel you need to add mpi++,mpicc,mpirun to PATH:
   		Firstly, run "find / -name "mpic++" (mpicc/mpirun are the same.)
   		Secondly, add the path by running "export PATH=$PATH:/usr/lib64/openmpi/bin/mpic++"
		Thirdly, add the mpirun to path by running "export PATH=/usr/lib64/openmpi/bin:$PATH"
   Use "mpic++ xxx.cpp -o xxx" to compile.
   Usage: mpirun -np 4 ./xxx   (4 represent the number of processes.)
*/

int main(int argc, char * argv[]){
	int rank;//The unique ID starting from 0 to identify each processes.
	int	size;//The size of process atually running.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("Hello world! I am the No.%d process in communicate field, total is %d.\n", rank, size);
	MPI_Finalize();//Verify the step at the end.
	return 0;
}
