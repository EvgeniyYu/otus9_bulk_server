#include "async.h"
#include "processor.h"

namespace async {

handle_t connect(std::size_t bulk)
{
	handle_t handle = Processor::get_instance().start_log_context(bulk);
	return handle;
}

void receive(handle_t handle, const char *data, std::size_t size)
{
	std::string str(data, size);
	Processor::get_instance().add_string(handle, str);
}

void disconnect(handle_t handle)
{
	Processor::get_instance().stop_log_context(handle);
}

void init(size_t size)
{
    Processor::get_instance().init(size);
}

}
