#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include "controller.h"

const size_t FILE_THREADS_COUNT = 2;

class LogInfo
{
public:
	LogInfo(size_t _size): size(_size) {}
	LogInfo() = default;
	std::vector<std::string> get_blocks(std::string& str_cmd);
private:
	std::string line;
	size_t size;
};


class Processor
{
	std::mutex mx;
	std::map<unsigned, LogInfo> log_contexts;
	size_t connect_count = 0;
	std::unique_ptr<IController> ctrl;
	bool is_init = false;
public:
	static Processor& get_instance();
	void init(size_t size);
    void* start_log_context(size_t size);
    void stop_log_context(void* handle);
    void add_string(void* handle, std::string& str);
private:
    Processor() = default;
	Processor(const Processor&) = delete;
	Processor& operator=(const Processor&) = delete;
	unsigned getRandomHandle();
};


#endif // PROCESSOR_H_INCLUDED
