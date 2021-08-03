#include "server_async.h"
#include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>



AsyncServer::AsyncServer(const size_t port, const size_t _bulk_size): bulk_size(_bulk_size)
															,_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    async::init(bulk_size);
	do_accept();
	io_context.run();
}


AsyncServer::~AsyncServer()
{
}


void AsyncServer::do_accept()
{
    _acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<Session>(std::move(socket), bulk_size)->start();
          }
          else
          {
          	std::cerr << "error code: " << ec.message() << std::endl;
          }

          do_accept();
        });
}


Session::Session(tcp::socket socket, const size_t _bulk_size): socket_(std::move(socket)), bulk_size(_bulk_size)
{
	h = async::connect(bulk_size);
}

Session::~Session()
{
}

void Session::start()
{
	do_read();
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                async::receive(h, data_, length);
                do_write(length);
            }
            else
            {
                std::cout << "disconnect handle" << std::endl;
                async::disconnect(h);
                //socket_.close();				//to add socket_.close() or not???
            }
        });
}


void Session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
          else
          {
          	std::cout << "disconnect handle" << std::endl;
          	async::disconnect(h);
          	//socket_.close();		//to add socket_.close() or not???
          }
        });
}

