#include "logger.h"

Logger::Logger() {}

void Logger::clear()
{
	message_log.clear();
}

void Logger::save(std::string filename)
{
	std::ofstream myfile;
    myfile.open (filename);
    myfile << "message_id, call, source, dest, tag, wait_time\n";
	int m_id = 0; 
    for (message m : message_log) {
		myfile << m_id << ", " << m.call << ", " << m.source << ", " << m.dest << ", " << m.tag << ", " << m.wait_time << "\n";
		m_id++;
	}
	myfile.close();
}

void Logger::log(message m)
{
	message_log.push_back(m);
}

void Logger::log(int c, int s, int d, int t, int w)
{
	message m = {c, s, d, t, w};
	log(m);
}

void Logger::emit_deadlock_detected(std::string method_name)
{
	printf("DEADLOCK DETECTED by %d in %s \n", rank, method_name.c_str());
	int noop = 0;
	for( int p = 0; p < size; p++ ) {
		if (p != rank) {
			MPI_Send(&noop, 1, MPI_INT, p, DEADLOCK_TAG, MPI_COMM_WORLD);
		}
	}
	REQUIRE(false);
}


bool Logger::is_deadlock_detected()
{
	int detected = false;
	MPI_Iprobe(MPI_ANY_SOURCE, DEADLOCK_TAG, MPI_COMM_WORLD, &detected, MPI_STATUS_IGNORE);
	if(detected) {
		int noop = 0;
		MPI_Recv(&noop, 1, MPI_INT, MPI_ANY_SOURCE, DEADLOCK_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		REQUIRE(false);
	}
	return detected;
}
