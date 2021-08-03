#include "processor.h"
#include <random>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

std::vector<std::string> LogInfo::get_blocks(std::string& str_cmd)
{
    std::vector<std::string> blocks, res;
    const std::string separator = "\n";
    boost::split(blocks, str_cmd, boost::is_any_of(separator));
    for (auto& value: blocks)
    {
        boost::algorithm::trim(value);
        if (value.empty()) continue;
        res.push_back(value);
    }
    return res;
}


Processor& Processor::get_instance()
{
    static Processor instance;
    return instance;
}

void* Processor::start_log_context(size_t size)
{
    std::lock_guard<std::mutex> lk(mx);

    unsigned handle = getRandomHandle();
    log_contexts[handle] = LogInfo(size);

    connect_count++;

    auto it = log_contexts.find(handle);
    unsigned* ptr = const_cast<unsigned*>(&(it->first));
    return (void*)(ptr);
}

void Processor::stop_log_context(void* handle)
{
    std::lock_guard<std::mutex> lk(mx);

    unsigned* u_handle = (unsigned*)(handle);
    log_contexts.erase(*u_handle);

    //
    connect_count--;
    if (!connect_count)
    {
    	//ctrl->stop();
        ctrl->join();
    }

}

void Processor::init(size_t size)
{
    std::lock_guard<std::mutex> lk(mx);

    if (is_init) return;
    ctrl = std::make_unique<Controller>(FILE_THREADS_COUNT, size);
    is_init = true;
}

void Processor::add_string(void* handle, std::string& str)
{
    std::lock_guard<std::mutex> lk(mx);

    unsigned* u_handle = (unsigned*)(handle);

    auto it = log_contexts.find(*u_handle);
    if (it == log_contexts.end())
    {
        std::cout << "handle not found" << std::endl;
        throw std::runtime_error("Error! Handle not found!");
    }

    std::vector<std::string> blocks = it->second.get_blocks(str);
    for (auto& str_block: blocks)
        ctrl->add_blocks(str_block);
}

unsigned Processor::getRandomHandle()
{
    std::mt19937 g;
    unsigned value;
    while (1)
    {
    	value = g();
    	if (log_contexts.find(value) == log_contexts.end()) break;
	}
	return value;
}
