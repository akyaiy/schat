#include <iostream>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "Config.h"
#include "Session.h"

int main(int argc, char** argv) {
	try {
		Config config("config.json", CONFIG_FORMAT_JSON);
		std::cout << "Config loaded successfully." << std::endl;

		auto data = config.GetData();

		boost::asio::io_context ioc;

		if (!(*data)["server"]["host"].is_string() ||
			!(*data)["server"]["port"].is_number()) 
		{
			throw std::runtime_error("Invalid server configuration in config file.");
		}

		std::string host = (*data)["server"]["host"];
		unsigned short port = (*data)["server"]["port"];

		tcp::acceptor acceptor(ioc, tcp::endpoint{
			boost::asio::ip::make_address(host),
			port
		});

		for (;;) {
			tcp::socket socket(ioc);
			acceptor.accept(socket);

			std::make_shared<Session>(std::move(socket))->start();
		}

	} catch (const std::exception& e) {
		std::cerr << "ERR: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
