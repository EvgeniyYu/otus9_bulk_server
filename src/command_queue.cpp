#include "command_queue.h"


size_t CommandQueue::size() const
{
    return queue.size();
}


void CommandQueue::push_back(const std::string& str)
{
    {
        std::unique_lock<std::mutex> lk(mx_ready_rw);
		queue.push(str);
    }
    cv.notify_all();
}


void CommandQueue::get_front(std::string& str)
{
    {
		std::unique_lock<std::mutex> lk(mx_ready_rw);
		if (!queue.size())
		{
			cv.wait(lk);
		}
		str = queue.front();
		queue.pop();
    }
}
