#ifndef APP_NETWORK_CONNECTION_H_
#define APP_NETWORK_CONNECTION_H_
#include <boost/asio.hpp>
#include <network/InitTreeHandler.h>
#include <network/MessageHandler.h>

class Connection {
public:
	Connection(std::shared_ptr<boost::asio::io_service> io_service);

	~Connection();

	typedef std::shared_ptr<Connection> connection_ptr;

	static connection_ptr create(std::shared_ptr<boost::asio::io_service> io_service);

	void connect(const boost::asio::ip::tcp::endpoint endpoint_iterator) {

		do_connect(endpoint_iterator);
	}

	template<class Message> void readMessage(std::shared_ptr<MessageHandler<Message> > msg_handler)	{

		std::vector<char> buf;

		do_read_msg(buf);

	    msg_handler->parseMessage(buf);
	}

	template<class Message> void write(std::shared_ptr<Message> msg) {

		std::vector<char> buffer;

		MessageParser<Message> parser(msg);

		parser.parseMsg(buffer);

		do_write_msg(buffer);
	}

	boost::asio::ip::tcp::socket& socket() {

		return socket_;
	}

private:
	boost::asio::ip::tcp::socket socket_;

	const uint HEADER_SIZE = 4;

	void do_connect(const boost::asio::ip::tcp::endpoint endpoint_it);

	void do_read_msg(std::vector<char>& buf);

	void do_write_msg(std::vector<char>& buf);

	int decode_header(const std::vector<char>& buf);
};

#endif /* APP_NETWORK_CONNECTION_H_ */
