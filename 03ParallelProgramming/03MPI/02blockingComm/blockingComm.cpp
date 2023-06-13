#include <mpi.h>
#include <stdio.h>

int main(int argc, char * argv[]){
	int rank, size, tag=1;
	int senddata, recvdata;
	MPI_Status status;
	int rc = MPI_Init(&argc, &argv);
	if(rc!=MPI_SUCCESS){
		fprintf(stderr, "Unable to set up MPI.");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank==0){
		senddata = 99999;
		for(rank=1;rank<size;rank++){
			MPI_Send(&senddata,1,MPI_INT,rank,tag,MPI_COMM_WORLD);
		}
	}
	else{
		MPI_Recv(&recvdata,1,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
		printf("The receive data from thread No.%d is: %d\n", rank, recvdata);
	}
	MPI_Finalize();
	return 0;
}
