#ifndef APP_NETWORK_SERVER_H_
#define APP_NETWORK_SERVER_H_
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <network/Connection.h>
#include <network/InitTreeHandler.h>
#include <network/HelloMessageHandler.h>
#include <network/AVLMessageHandler.h>
#include <network/DummyMessageHandler.h>
#include <network/RemoteTreeManager.h>
#include <network/RootDutiesManager.h>
#include <network/Sender.h>

class Server {
public:
	Server( std::shared_ptr<boost::asio::io_service> io_service,
			boost::asio::ip::tcp::endpoint endpoint,
			std::shared_ptr<InitTreeHandler> init_handler,
			std::shared_ptr<HelloMessageHandler> hello_handler,
			std::shared_ptr<AVLMessageHandler> avl_handler,
			std::shared_ptr<DummyMessageHandler> dummy_handler,
			std::shared_ptr<Sender> sender,
			bool root = false);

	void start();

	void sayHello();
	void getHello();
	void resendDummy();

	std::shared_ptr<RemoteTreeManager> getTreeManager() {
		return tree_manager_;
	}

private:

	std::shared_ptr<boost::asio::io_service> io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::endpoint endpoint_;
	std::shared_ptr<boost::asio::strand> strand_;

	std::shared_ptr<InitTreeHandler> init_handler_;
	std::shared_ptr<HelloMessageHandler> hello_handler_;
	std::shared_ptr<AVLMessageHandler> avl_handler_;
	std::shared_ptr<DummyMessageHandler> dummy_handler_;

	std::shared_ptr<RemoteTreeManager> tree_manager_;
	std::shared_ptr<RootDutiesManager> duties_manager_;

	std::shared_ptr<Sender> sender_;

	bool root_;

	void do_accept();

	void handle_accept(Connection::connection_ptr new_connection,
	    const boost::system::error_code& error);
};

#endif /* APP_NETWORK_SERVER_H_ */
