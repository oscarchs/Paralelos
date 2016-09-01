#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  int rank;
  MPI_Comm_rank(communicator, &rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if (rank == root) {
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {
    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

void RAP_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
  int i;
  int rank;
  MPI_Comm_rank(communicator, &rank);
    int size;
    MPI_Comm_size(communicator, &size);
  if(root != 0) {
    if(rank == root)
      MPI_Send(data, count, datatype, 0, 0, MPI_COMM_WORLD);
    else if(rank == 0)
      MPI_Recv(data, count, datatype, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if(rank > 0) {
    if(rank % 2 != 0)
      MPI_Recv(data, count, datatype, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    else {
      int exp = log2(rank), lowBound = pow(2,exp), upperBound = pow(2,exp+1);
      //printf("%d\t,%d\t,%d\t,%d\n",rank,exp,lowBound,upperBound);
      if(isPowerOfTwo(rank))
        MPI_Recv(data, count, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      else 
        MPI_Recv(data, count, datatype, lowBound, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      for (i = 1; rank + i < upperBound && rank + i < size; i*=2) {
          MPI_Send(data, count, datatype, rank + i, 0, MPI_COMM_WORLD);
        }
    }
  }
  else {
    for (i = 1; i < size; i*=2)
      MPI_Send(data, count, datatype, i, 0, MPI_COMM_WORLD);
  }
}


int main(int argc, char** argv) {
  
  MPI_Init(NULL, NULL);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int data;
  if (rank == 0) {
    data = 100;
    printf("Process 0 broadcasting data %d\n", data);
    my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received data %d from root process\n", rank, data);
  }

  MPI_Finalize();

}