#include "parallel.h"

int race_condition(int data)
{
//    MPI_Init(NULL, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (data <= 0) {
        if (rank == 0) {
            printf("Data (%d) must be greater than 0\n", data);
        }
        return data - 1;
    }

    MPI_Request req;
    MPI_Status stat;

    if (size > 2) {
        if (rank == 0) {
            printf("Too many processes (%d) for this program", size);
        }
        return -1;
    }

    int source = 0;
    int dest = 1;
    int a = 0;

    if (rank == 0) {
        a = data;
        // Start immediate send
        MPI_Isend (&a, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, &req);
        a = -1;
        MPI_Wait (&req, &stat);
        // Has the dest process received 1 or -1?
        a = data;
    }
    else {
        MPI_Recv(&a, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &stat);
    }

//    MPI_Finalize();
    return a;
}
