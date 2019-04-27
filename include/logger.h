#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <catch2/catch.hpp>
#include "mpi.h"

enum call {MPIc_SEND, MPIc_ISEND, MPIc_RECV, MPIc_IRECV, MPIc_BCAST, MPIc_IBCAST };

struct message {
	int call;
	int source;
	int dest;
	int tag;
	int wait_time;
};

class Logger {
	private:
		int DEADLOCK_TAG = 2606;
	public:
		Logger();

		std::vector<message> message_log;

		int threshold;
		int rank;
		int size;
		int max_wait_time;
		bool use_wrapper;
	
		void clear();
		void save(std::string filename);
		void log(message m);
		void log(int c, int s, int d, int t, int w);
		void emit_deadlock_detected(std::string method_name);
		bool is_deadlock_detected();
};
