#ifndef SERVER_ASYNC_H
#define SERVER_ASYNC_H

#include "async.h"
#include <boost/asio.hpp>
#include <memory>
#include <utility>


using boost::asio::ip::tcp;
using namespace async;

class Session: public std::enable_shared_from_this<Session>
{
    tcp::socket socket_;
    const size_t bulk_size;
    handle_t h;
    enum { max_length = 1024 };
    char data_[max_length];
public:
    Session(tcp::socket socket, const size_t _bulk_size);
    ~Session();
    void start();
private:
    void do_read();
    void do_write(std::size_t length);
};


class AsyncServer
{
    const size_t bulk_size;
    boost::asio::io_context io_context;
    tcp::acceptor _acceptor;
public:
    AsyncServer(const size_t _port, const size_t _bulk_size);
    ~AsyncServer();
    void do_accept();
};




#endif
