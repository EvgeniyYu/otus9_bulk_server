#ifndef COMMAND_QUEUE_H_INCLUDED
#define COMMAND_QUEUE_H_INCLUDED

#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <fstream>

class CommandQueue
{
	std::queue<std::string> queue;
	std::mutex mx_ready_rw;
	std::condition_variable cv;
	std::ofstream ofs;
public:
	CommandQueue() {}
	~CommandQueue() {}

	size_t size() const;
	void push_back(const std::string& str);
	void get_front(std::string& str);
};

#endif // COMMAND_QUEUE_H_INCLUDED
