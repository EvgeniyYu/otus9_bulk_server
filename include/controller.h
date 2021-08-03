#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <vector>
#include <memory>
#include "logger_view.h"

class Observable
{
public:
	void add_observer(std::shared_ptr<LoggerView> sptr_obs);
	void notify(const std::string& str);
protected:
	std::vector<std::shared_ptr<LoggerView>> observers;
};


class IController: public Observable
{
public:
	virtual void add_blocks(std::string& str_block) = 0;
	virtual void add_string(std::string& str) = 0;
	virtual void join() = 0;
	virtual void stop() = 0;
};

class Controller: public IController
{
	std::shared_ptr<LoggerView> cview;
	std::shared_ptr<LoggerView> fview;
public:
	Controller(const size_t wf_thread_count, const size_t size);
	void add_blocks(std::string& str_block) override;
	void add_string(std::string& str) override;
	void join() override;
	void stop() override;
};


#endif // CONTROLLER_H_INCLUDED
