#include "Session.h"

Session::Session(tcp::socket socket)
	: socket_(std::move(socket))
{
	try {
		auto ep = socket_.remote_endpoint();
		client_ip_ = ep.address().to_string();
		client_port_ = ep.port();
	}
	catch (...) {
		client_ip_ = "unknown";
		client_port_ = 0;
	}
}

void Session::start() {
	auto self = shared_from_this();
	std::thread([self]() { self->run(); }).detach();
}

void Session::run() {
	try {
		std::cout << "Client connected: " 
				<< client_ip_ << ":" << client_port_ << std::endl;

		boost::beast::flat_buffer buffer;
		boost::beast::http::request<boost::beast::http::string_body> req;

		// Read HTTP upgrade request with error handling
		boost::beast::error_code ec;
		boost::beast::http::read(socket_, buffer, req, ec);
		if (ec) {
			std::cerr << "HTTP read error (" << client_ip_ << "): " 
					<< ec.message() << std::endl;
			return;
		}

		// Create WebSocket and accept handshake
		websocket::stream<tcp::socket> ws(std::move(socket_));
		ws.accept(req, ec);
		if (ec) {
			std::cerr << "WebSocket accept error (" << client_ip_ << "): " 
					<< ec.message() << std::endl;
			return;
		}

		std::cout << "Handshake OK: "
				<< client_ip_ << ":" << client_port_ << std::endl;

		// Echo loop
		for (;;) {
			boost::beast::flat_buffer buf;

			ws.read(buf, ec);
			if (ec) {
				if (ec == websocket::error::closed) {
					std::cout << "Client disconnected gracefully: "
							<< client_ip_ << ":" << client_port_ << std::endl;
				}
				else if (ec == boost::asio::error::connection_reset ||
							ec == boost::asio::error::eof) {
					std::cout << "Client connection reset: "
							<< client_ip_ << ":" << client_port_ << std::endl;
				}
				else {
					std::cerr << "Read error (" << client_ip_ << "): " 
							<< ec.message() << std::endl;
				}
				break;
			}

			ws.text(ws.got_text());
			ws.write(buf.data(), ec);
			if (ec) {
				if (ec == boost::asio::error::connection_reset ||
					ec == boost::asio::error::eof) {
					std::cout << "Client connection lost during write: "
							<< client_ip_ << ":" << client_port_ << std::endl;
				}
				else {
					std::cerr << "Write error (" << client_ip_ << "): " 
							<< ec.message() << std::endl;
				}
				break;
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Client error (" << client_ip_ << "): "
				<< e.what() << std::endl;
	}
}
