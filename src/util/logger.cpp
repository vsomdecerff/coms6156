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
    myfile << "call, source, dest, tag, wait_time\n";
    for (message m : message_log) {
		myfile << m.call << ", " << m.source << ", " << m.dest << ", " << m.tag << ", " << m.wait_time << "\n";
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

void Logger::deadlock()
{}

