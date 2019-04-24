#include <iostream>
#include <fstream>
#include <vector>
#include <string>

enum call {MPI_SEND, MPI_ISEND, MPI_RECV };

struct message {
	int call;
	int source;
	int dest;
	int tag;
	int wait_time;
};

class Logger {

	private:
		std::vector<message> message_log;
	
	public:
		Logger();

		int threshold;
		int rank;
		int max_wait_time;

		void clear();
		void save(std::string filename);
		void log(message m);
		void log(int c, int s, int d, int t, int w);
		void deadlock();
};
