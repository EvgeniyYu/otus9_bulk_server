#include "controller.h"

void Observable::add_observer(std::shared_ptr<LoggerView> sptr_obs)
{
	observers.push_back(sptr_obs);
}

void Observable::notify(const std::string& str)
{
	for (const auto& obs: observers)
	{
		obs->update(str);
	}
}


Controller::Controller(const size_t wf_thread_count, const size_t size)
{
	cview = std::make_shared<ConsoleView>(size);
	fview = std::make_shared<FileView>(wf_thread_count, size);
	cview->start();
	fview->start();
	add_observer(cview);
	add_observer(fview);
}

void Controller::add_blocks(std::string& str_block)
{
	notify(str_block);
}

void Controller::add_string(std::string& str)
{
    notify(str);
}

void Controller::join()
{
	for (const auto& obs: observers)
		obs->join();
}

void Controller::stop()
{
	for (const auto& obs: observers)
		obs->stop();
}
