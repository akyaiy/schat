#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <thread>
#include <iostream>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

class Session : public std::enable_shared_from_this<Session> {
public:
	explicit Session(tcp::socket socket);

	void start();

private:
	void run();

	tcp::socket socket_;
	std::string client_ip_;
	unsigned short client_port_;
};