#include "deadlock.h"

int deadlock_1(int data)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int dest = 0;
    int a = 0;

    if (rank == 0) {
    	for (int i = 1; i < size; i++) {
			MPI_Wrap::MPIw_Recv(&a, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
    else {
		a = rank * data;
        MPI_Wrap::MPIw_Send(&a, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);	
    }
	
	return data;
}

int deadlock_2(unsigned int data)
{
	int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size != 2) {
        if (rank == 0) {
            printf("Only 2 processes (%d) for this program", size);
        }
        return -1;
    }

	int a = data - 1;
	int b = -100;
	if (rank == 0) {
		MPI_Wrap::MPIw_Send (&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		// MPI does not guarantee that the execution won't block inside MPI_Send
		MPI_Wrap::MPIw_Recv (&b, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else if (rank == 1) {
		MPI_Wrap::MPIw_Send (&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // MPI does not guarantee that the execution won't block inside MPI_Send
        MPI_Wrap::MPIw_Recv (&b, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	return b + 1;
}

int deadlock_3(int data)
{   
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int a = 0;
  
    if (rank == 0) { 
		a = data;
        for (int i = 1; i < size; i++) {
			MPI_Wrap::MPIw_Send(&a, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
    }
    
    return a;
}

int deadlock_4(int data)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a = 0;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Wrap::MPIw_Recv (&a, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else {
    }

    return a;
}
