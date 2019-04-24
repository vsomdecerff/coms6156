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
//	printf("logged %d", message_log.size());
}

void Logger::log(int c, int s, int d, int t, int w)
{
	message m = {c, s, d, t, w};
	log(m);
}

void Logger::deadlock()
{}

