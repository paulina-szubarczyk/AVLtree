#include <network/Connection.h>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "glog/logging.h"

Connection::Connection(std::shared_ptr<boost::asio::io_service> io_service) : socket_(*io_service){
}

Connection::~Connection() {

	LOG(INFO) << __FUNCTION__;

	LOG(INFO) << "IP: " << socket_.remote_endpoint().address().to_string() << " PORT: " << socket_.remote_endpoint().port();

	socket_.close();
}

Connection::connection_ptr Connection::create(std::shared_ptr<boost::asio::io_service> io_service) {

	LOG(INFO) << __FUNCTION__;

	return std::make_shared<Connection>(io_service);
}


void Connection::do_connect(const boost::asio::ip::tcp::endpoint endpoint_iterator) {

	LOG(INFO) << __FUNCTION__;

	boost::asio::ip::tcp::resolver::iterator end;

	boost::system::error_code error = boost::asio::error::host_not_found;

    socket_.connect(endpoint_iterator, error);

	LOG(INFO) << "Connected to: IP: " << socket_.remote_endpoint().address().to_string() << " PORT: " << socket_.remote_endpoint().port();

	if (error)
	  throw boost::system::system_error(error);
}


void Connection::do_read_msg(std::vector<char>& buf) {

//	LOG(INFO) << __FUNCTION__;

	std::vector<char> header(HEADER_SIZE);

    boost::system::error_code error;

	boost::asio::read(socket_, boost::asio::buffer(header,HEADER_SIZE), error);

	int len = decode_header(header);

    buf.resize(len);

    boost::asio::read(socket_, boost::asio::buffer(buf, len), error);

//    LOG(INFO) << "msg size " << len;

    if (error != boost::asio::error::eof && error != boost::system::errc::success ) {

//    	LOG(WARNING) << __FUNCTION__ << " Error: " << error;

    	throw boost::system::system_error(error); // Some other error.
    }
    else {

//    	LOG(WARNING) << __FUNCTION__ << " Error: " << error;

    }
}

void Connection::do_write_msg(std::vector<char>& buf) {

//	LOG(INFO) << __FUNCTION__ ;

	boost::system::error_code ignored_error;

    boost::asio::write(socket_, boost::asio::buffer(buf), boost::asio::transfer_all(), ignored_error);

	if (ignored_error) {

		std::cout << "Got exception: " << ignored_error;
	}

}

int Connection::decode_header(const std::vector<char>& buf) {

//	LOG(INFO) << __FUNCTION__;

	if (buf.size() < HEADER_SIZE) {

		throw new std::logic_error("Buffer's size should be >= 4");
	}

	unsigned msg_size = 0;

	for (unsigned i = 0; i < HEADER_SIZE; ++i) {

		msg_size = msg_size * 256 + (static_cast<unsigned>(buf[i]) & 0xFF);
	}

	return msg_size;
}

