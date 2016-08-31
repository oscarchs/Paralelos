#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int rank;
int size;
const int limit = 5;

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int count = 0;
	int adversary = (rank + 1) % 2;
	while (count < limit) {
    if (rank == count % 2) {
        // Increment the ping pong count before you send it
        count++;
        MPI_Send(&count, 1, MPI_INT, adversary, 0,
                 MPI_COMM_WORLD);
        printf("%d sent and incremented count "
               "%d to %d\n", rank, count,
               adversary);
    } else {
        MPI_Recv(&count, 1, MPI_INT, adversary, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%d received count %d from %d\n",
               rank, count, adversary);
    }
}
	MPI_Finalize();
	return 0;
}