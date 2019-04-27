#pragma once
#include <mpi.h>
#include <stdlib.h>
#include <time.h> 
#include <thread>
#include <chrono>
#include "logger.h"

namespace MPI_Wrap {

	Logger log;

	void use_wrapper(bool use) {
		log.use_wrapper = use;
	}

	void set_rank(int rank) {
		log.rank = rank;
	}
	
	void set_size(int size) {
		log.size = size;
	}

	void set_threshold(int threshold, int max_wait_time) {
		if (max_wait_time == 0) {
			max_wait_time = 1;
		}
		if (threshold == 0) {
			threshold = 1;
		}
		if ( max_wait_time >= threshold) {
			threshold = max_wait_time + 1 ;
		}
		log.threshold = threshold;
		log.max_wait_time = max_wait_time;
	}

	void clear_log() {
		srand (time(NULL));
		log.clear();
	}

	void write_log(std::string filename) {
		log.save(filename);
	}

	void check_deadlock_message() {
		log.is_deadlock_detected();
	}

	int time() {
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = now.time_since_epoch();
		auto time_cast = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
		return time_cast;
	}

    int MPIw_Init(int *argc, char ***argv) {
        return MPI_Init(argc, argv);
    }

	int MPIw_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request) {
		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

		if (log.is_deadlock_detected()) 
		{
			return MPI_ERR_UNKNOWN;
		}
		log.log(MPIc_ISEND, log.rank, dest, tag, wait_time);
		return MPI_Isend(buf, count, datatype, dest, tag, comm, request);	
	}

    int MPIw_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                 int tag, MPI_Comm comm) {
        int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));


		if (log.is_deadlock_detected())
        {
            return MPI_ERR_UNKNOWN;
        }

	    log.log(MPIc_SEND, log.rank, dest, tag, wait_time);
	
		if(!log.use_wrapper) {
			return MPI_Send(buf, count, datatype, dest, tag, comm);
		}

		int ret_value;

		MPI_Request req;
		MPI_Status stat;

		int start = time();
		ret_value = MPI_Isend(buf, count, datatype, dest, tag, comm, &req);

		int flag = false;
		while(!flag) {
			ret_value = MPI_Test(&req, &flag, &stat);
			if (ret_value != MPI_SUCCESS) {
				return ret_value;
			}

			if (log.is_deadlock_detected())
        	{
            	return MPI_ERR_UNKNOWN;
        	}

			if( (time() - start) > log.threshold) {
				log.emit_deadlock_detected("MPI_Send");
				return MPI_ERR_UNKNOWN;
			}
		}

		return ret_value;
    }

	int MPIw_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request * request) {

		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

		if (log.is_deadlock_detected())
        {
            return MPI_ERR_UNKNOWN;
        }

        log.log(MPIc_IRECV, source, log.rank, tag, wait_time);

        return MPI_Irecv(buf, count, datatype, source, tag, comm, request);
	}

    int MPIw_Recv(void *buf, int count, MPI_Datatype datatype, int source,
             int tag, MPI_Comm comm, MPI_Status *status) {

		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

		int ret_value ;

		MPI_Status stat;
		if (status == MPI_STATUS_IGNORE) {
			status = &stat;	
		}

		if(!log.use_wrapper) {
			ret_value = MPI_Recv(buf, count, datatype, source, tag, comm, status);
			log.log(MPIc_RECV, status->MPI_SOURCE, log.rank, tag, wait_time);
			return ret_value;
		}
		
        int start = time();

		int flag = false;
        while(!flag) {
			ret_value = MPI_Iprobe(source, tag, comm, &flag, MPI_STATUS_IGNORE);
            if (ret_value != MPI_SUCCESS) {
                return ret_value;
            }

			if (log.is_deadlock_detected())
        	{
            	return MPI_ERR_UNKNOWN;
        	}

            if( (time() - start) > log.threshold) {
                log.emit_deadlock_detected("MPI_Recv");
                return MPI_ERR_UNKNOWN;
            }
        }
		ret_value = MPI_Recv(buf, count, datatype, source, tag, comm, status);
		log.log(MPIc_RECV, status->MPI_SOURCE, log.rank, tag, wait_time);
        return ret_value;
    }

	int MPIw_Ibcast(void *buffer, int count, MPI_Datatype datatype, int root, 
			MPI_Comm comm, MPI_Request *request) {

		int wait_time = rand() % log.max_wait_time;
		std::this_thread::sleep_for (std::chrono::seconds(wait_time));
		
		if (log.is_deadlock_detected()) 
		{
			return MPI_ERR_UNKNOWN;
		}

		log.log(MPIc_IBCAST, root, log.rank, 0, wait_time);
		return MPI_Ibcast(buffer, count, datatype, root, comm, request); 
	}

	int MPIw_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, 
               MPI_Comm comm ) {

		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

        if (log.is_deadlock_detected())
        {
            return MPI_ERR_UNKNOWN;
        }

        log.log(MPIc_BCAST, root, log.rank, 0, wait_time);

		if(!log.use_wrapper) {
            return MPI_Bcast(buffer, count, datatype, root, comm);
        }

        int ret_value;

        MPI_Request req;
        MPI_Status stat;

        int start = time();
        ret_value = MPI_Ibcast(buffer, count, datatype, root, comm, &req);

        int flag = false;
        while(!flag) {
            ret_value = MPI_Test(&req, &flag, &stat);
            if (ret_value != MPI_SUCCESS) {
                return ret_value;
            }

            if (log.is_deadlock_detected())
            {
                return MPI_ERR_UNKNOWN;
            }

            if( (time() - start) > log.threshold) {
                log.emit_deadlock_detected("MPI_Bcast");
                return MPI_ERR_UNKNOWN;
            }
        }

        return ret_value;
	}
}       
