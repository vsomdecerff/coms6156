#include <mpi.h>
#include <stdlib.h>
#include <time.h> 
#include <thread>
#include <chrono>
#include "logger.h"

namespace MPI_Wrap {

	Logger log; 
	int a =0;

	void call() {
        a++;
        std::cout << a << "\n";
		log.log(MPI_SEND, 0, 1, a, 12);
		log.save("tmp.csv");
    }

	void set_rank(int rank) {
		log.rank = rank;
	}

	void set_threshold(int threshold, int max_wait_time) {
		if (threshold > max_wait_time) {
			max_wait_time = threshold;
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

	int time() {
		auto now = std::chrono::high_resolution_clock::now();
		auto duration = now.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		return millis;
	}

    int MPIw_Init(int *argc, char ***argv) {
        return MPI_Init(argc, argv);
    }

	int MPIw_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
              MPI_Comm comm, MPI_Request *request) {
		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

        log.log(MPI_ISEND, log.rank, dest, tag, wait_time);

		return MPI_Isend(buf, count, datatype, dest, tag, comm, request);	
	}

    int MPIw_Send(const void *buf, int count, MPI_Datatype datatype, int dest,
                 int tag, MPI_Comm comm) {
        int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));
   
	    log.log(MPI_SEND, log.rank, dest, tag, wait_time);
	
		int ret_value;

		MPI_Request req;
		MPI_Status stat;

		int start = time();
		ret_value = MPI_Isend(buf, count, datatype, dest, tag, comm, &req);

		int flag = false;
		while(flag) {
			ret_value = MPI_Test(&req, &flag, &stat);
			if (ret_value != MPI_SUCCESS) {
				return ret_value;
			}

			if( (time() - start) > log.threshold) {
				log.deadlock();
				return MPI_ERR_UNKNOWN;
			}
		}

		return ret_value;
    }

	int MPIw_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
              int tag, MPI_Comm comm, MPI_Request * request) {

		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

        log.log(MPI_ISEND, source, log.rank, tag, wait_time);

        return MPI_Irecv(buf, count, datatype, source, tag, comm, request);
	}

    int MPIw_Recv(void *buf, int count, MPI_Datatype datatype, int source,
             int tag, MPI_Comm comm, MPI_Status *status) {

		int wait_time = rand() % log.max_wait_time;
        std::this_thread::sleep_for (std::chrono::seconds(wait_time));

        log.log(MPI_ISEND, source, log.rank, tag, wait_time);
	
		MPI_Request req;
        MPI_Status stat;

        int start = time();
		int ret_value = MPI_Irecv(buf, count, datatype, source, tag, comm, &req);

		int flag = false;
        while(flag) {
            ret_value = MPI_Test(&req, &flag, &stat);
            if (ret_value != MPI_SUCCESS) {
                return ret_value;
            }

            if( (time() - start) > log.threshold) {
                log.deadlock();
                return MPI_ERR_UNKNOWN;
            }
        }

        return ret_value;
    }
}       
