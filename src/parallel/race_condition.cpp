#include "race_condition.h"

unsigned int race_condition_1(unsigned int data)
{
//    MPI_Init(NULL, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Request req;
    MPI_Status stat;

    if (size != 2) {
        if (rank == 0) {
            printf("Only 2 processes (%d) for this program", size);
        }
        return data + 1;
    }

    int source = 0;
    int dest = 1;
    unsigned int a = 0;

    if (rank == 0) {
        a = data;
        // Start immediate send
        MPI_Wrap::MPIw_Isend (&a, 1, MPI_UNSIGNED, dest, 0, MPI_COMM_WORLD, &req);
        a = -1;
        // Has the dest process received 1 or -1?
        a = data;
    }
    else if (rank == 1) {
        MPI_Wrap::MPIw_Recv(&a, 1, MPI_UNSIGNED, source, 0, MPI_COMM_WORLD, &stat);	
    }

//    MPI_Finalize();
    return a;
}

unsigned int race_condition_2(unsigned int data)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Request req;
    MPI_Status stat;

    if (size != 2) {
        if (rank == 0) {
            printf("Only 2 processes (%d) for this program", size);
        }
        return data + 1;
    }

    int source = 0;
    int dest = 1;
    unsigned int a[data];

    if (rank == 0) {
        for (int i = 0; i < data; i++) {
			a[i] = data;
		}
        // Start immediate send
        MPI_Wrap::MPIw_Isend (&a, data, MPI_UNSIGNED, dest, 0, MPI_COMM_WORLD, &req);
    }
    else if (rank == 1) {
		for (int i = 0; i < data; i++) {
            a[i] = 0;
        }
        MPI_Wrap::MPIw_Irecv(&a, data, MPI_UNSIGNED, source, 0, MPI_COMM_WORLD, &req);

    }

    return a[0];
}

unsigned int race_condition_3(unsigned int data)
{
	int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size != 3) {
        if (rank == 0) {
            printf("Only 3 processes (%d) for this program", size);
        }
        return data + 1;
    }

	int a = 0;
	int b = rank;
	int dest = 0;
	if (rank == 0) {
		MPI_Wrap::MPIw_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if ( a != 1) { b++; }
		MPI_Bcast(&rank, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Wrap::MPIw_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if ( a != 2) { b++; }
	}
	else if (rank == 1) {
		MPI_Wrap::MPIw_Send(&rank, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else if (rank == 2) {
		MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Wrap::MPIw_Send(&rank, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
	}
	return data + b;
}
