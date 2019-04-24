#pragma once
#include <mpi.h>
#include <stdlib.h>
#include <time.h> 
#include <thread>
#include <chrono>
#include "logger.h"

namespace MPI_Wrap {

	void use_wrapper(bool use);
	void set_rank(int rank);
	void set_threshold(int threshold, int max_wait_time);
	void clear_log();

	void write_log(std::string filename) ;

	int time() ;

    int MPIw_Init(int *argc, char ***argv) ;

	int MPIw_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request);

    int MPIw_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                 int tag, MPI_Comm comm);

	int MPIw_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request * request);

    int MPIw_Recv(void *buf, int count, MPI_Datatype datatype, int source,
             int tag, MPI_Comm comm, MPI_Status *status);
}       
