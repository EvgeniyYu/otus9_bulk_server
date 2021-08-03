#include "logger_view.h"

#include <ctime>
#include <random>
#include <filesystem>
#include <iostream>
#include <string>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>


namespace fs = std::filesystem;


ConsoleView::ConsoleView(size_t _size)
{
    this->size = _size;
}

void ConsoleView::start()
{
	if (started) return;
	started = true;
	th = std::thread ([&]()
	{
		while (1)
		{
			if (!started && !cmd_queue.size()) break;
			{
				std::string str_cmd;
				this->cmd_queue.get_front(str_cmd);
				write(str_cmd);
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});
}

void ConsoleView::stop()
{
	started = false;
}

void ConsoleView::join()
{
	th.join();
}


void ConsoleView::write(const std::string& str)
{
    static size_t cnt = 0;
    if (!cnt) std::cout << "bulk: ";
    if (cnt < this->size)
    {
        std::cout << str;
        if (cnt < this->size - 1) std::cout << ", " << std::flush;
        cnt++;
    }
    else
    {
        std::cout << "\n" << "bulk: " << str << ", " << std::flush;
        cnt = 1;
    }

}

void ConsoleView::update(const std::string& str)
{
	this->cmd_queue.push_back(str);
}


FileView::FileView(size_t _threads_count, size_t _size): threads_count(_threads_count)
{
    this->size = _size;
}



void FileView::update(const std::string& str)
{
	cmd_queue.push_back(str);
}

void FileView::start()
{

	if (started) return;
	started = true;
	for (size_t i = 0; i < threads_count; i++)
		threads.emplace_back([&]()
		{
			while (1)
			{
				if (!started && !cmd_queue.size()) break;
				{
					std::string str_cmd;

					this->cmd_queue.get_front(str_cmd);

					write(str_cmd);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
}

void FileView::stop()
{
	started = false;
}

void FileView::join()
{
	for (auto& th: threads)
	{
		th.join();
	}
}

void FileView::write(const std::string& str)
{
    std::lock_guard<std::mutex> lock(mx_write);

    static std::atomic<size_t> cnt = 0;
    if (!cnt) sstr << "bulk: ";
    if (cnt < this->size)
    {
        sstr << str;
        if (cnt < this->size - 1) sstr << ", ";
        cnt.fetch_add(1);
    }
    else
    {
        ofs.close();
        sstr.str("");
        sstr << "bulk: " << str << ", ";
        cnt = 1;
    }

    if (cnt == 1)
    {
        create_file();
    }
    save_bulk(sstr.str());
    sstr.str("");
}

void FileView::create_file()
{
    const std::string dir_name = "Log";
	std::string dir_path = fs::current_path() / dir_name;
	fs::directory_entry entry_dir { dir_path };
    if (!entry_dir.exists())
    {
        if (!fs::create_directory(dir_path))
        {
        	throw std::runtime_error("Error creating log directory");
    	}
    }

    static size_t cnt = 0;
    std::time_t result = std::time(nullptr);
    std::stringstream sstr_name;
    sstr_name << dir_name <<"/log_" << result << "_" << ++cnt;
	std::string fname = sstr_name.str();
    ofs.open(fname, std::ios::app);
    if (!ofs.is_open())
    {
        std::cerr << "error open file for data output" << std::endl;
        throw std::runtime_error("Exception error! File cannot be open for data output!");
    }
}

void FileView::save_bulk(const std::string& str)
{
    ofs << str << std::flush;
}
