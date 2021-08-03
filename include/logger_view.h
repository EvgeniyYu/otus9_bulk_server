#ifndef LOGGER_VIEW_H_INCLUDED
#define LOGGER_VIEW_H_INCLUDED

#include "command_queue.h"
#include <fstream>
#include <thread>
#include <mutex>
#include <memory>
#include <sstream>
#include <atomic>

class LoggerView
{
protected:
	CommandQueue cmd_queue;
	size_t size;
public:
	virtual void update(const std::string&) = 0;
	virtual void start() = 0;
	virtual void stop()= 0;
	virtual void join() = 0;
};


class ConsoleView: public LoggerView
{
	std::thread th;
	std::atomic<bool> started{false};
public:
	ConsoleView(size_t _size);
	void update(const std::string&) override;
	void start() override;
	void stop() override;
	void join() override;
private:
	void write(const std::string&);
};


class FileView: public LoggerView
{
	std::vector<std::thread> threads;
	size_t threads_count;
	std::atomic<bool> started{false};
	std::mutex mx_write;
	std::ofstream ofs;
	std::stringstream sstr;
public:
	FileView(size_t _threads_count, size_t _size);
	void update(const std::string&) override;
	void start() override;
	void stop() override;
	void join() override;
private:
	void write(const std::string&);
	void save_bulk(const std::string& str);
	void create_file();
};


#endif // LOGGER_VIEW_H_INCLUDED
