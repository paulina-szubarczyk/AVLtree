#include <network/Server.h>
#include <network/Connection.h>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include "glog/logging.h"

Server::Server(std::shared_ptr<boost::asio::io_service> io_service,
		boost::asio::ip::tcp::endpoint endpoint,
		std::shared_ptr<InitTreeHandler> init_handler,
		std::shared_ptr<HelloMessageHandler> hello_handler,
		std::shared_ptr<AVLMessageHandler> avl_handler,
		std::shared_ptr<DummyMessageHandler> dummy_handler,
		std::shared_ptr<Sender> sender,
		bool root)
		: io_service_(io_service), acceptor_(*io_service, endpoint), socket_(*io_service), endpoint_(endpoint),
		  init_handler_(init_handler), hello_handler_(hello_handler), avl_handler_(avl_handler), dummy_handler_(dummy_handler),
		  sender_(sender), root_(root)
{
	LOG(INFO) << __FUNCTION__;

	strand_ = std::make_shared<boost::asio::strand>(*io_service);

	tree_manager_ = std::make_shared<RemoteTreeManager> (io_service, strand_, avl_handler_);

	avl_handler_->setTree(tree_manager_);

	duties_manager_ = std::make_shared<RootDutiesManager> (io_service, strand_, init_handler->getNodeCounter());

	duties_manager_->setRootHandler(std::make_shared<RootMessageHandler>(tree_manager_->getAvlTree()));

	if (root) {
		tree_manager_->addRootConnection(nullptr);
	}
}

void Server::start() {

	LOG(INFO) << __FUNCTION__;

	init_handler_->notifyWhenReady(boost::bind(&Server::sayHello, this));

	hello_handler_->notifyWhenReady(boost::bind(&Server::getHello, this));

	dummy_handler_->notifyWhenReady(boost::bind(&Server::resendDummy, this));

	dummy_handler_->notifyWhenReady(boost::bind(&RemoteTreeManager::notify, tree_manager_));

	do_accept();
}

void Server::do_accept()
{
	LOG(INFO) << __FUNCTION__;

	Connection::connection_ptr new_connection = Connection::create(io_service_);

	acceptor_.async_accept(new_connection->socket(),
	          boost::bind(&Server::handle_accept, this, new_connection,
	          boost::asio::placeholders::error));
}

void Server::handle_accept(Connection::connection_ptr new_connection,
	    const boost::system::error_code& error)
{
	LOG(INFO) << __FUNCTION__;

	if (!error )
	{
		if(root_ && !init_handler_->ready()) {

			duties_manager_->add_connection(new_connection);

			strand_->post(boost::bind( &Connection::readMessage<sr::InitSystemMessage>, new_connection,
										std::dynamic_pointer_cast<MessageHandler<sr::InitSystemMessage> >(init_handler_)));

		} else if (!hello_handler_->ready()) {

			sender_->setConnection(new_connection);
//
//			strand_->post(boost::bind( &Connection::write<sr::Hello>, new_connection,
//										std::dynamic_pointer_cast<MessageHandler<sr::Hello> >(hello_handler_)));

		} else {

			tree_manager_->addRootConnection(new_connection);

			strand_->post(boost::bind( &Connection::readMessage<sr::Message>, new_connection,
									    std::dynamic_pointer_cast<MessageHandler<sr::Message> >(avl_handler_)));
		}

		do_accept();
	}

}

void Server::sayHello() {

	LOG(INFO) << __FUNCTION__;

	duties_manager_->createConnection(init_handler_->getEndpointMap(), endpoint_);

	for(auto new_connection : duties_manager_->getConnections()) {
		io_service_->post(boost::bind(&Connection::write<sr::Hello>, new_connection, init_handler_->getHelloMsg()));
	}

	duties_manager_->waitForDummy(dummy_handler_);
}

void Server::resendDummy() {

	LOG(INFO) << __FUNCTION__;

	for(auto new_connection : duties_manager_->getConnections()) {
		io_service_->post(boost::bind(&Connection::write<sr::Dummy>, new_connection, std::make_shared<sr::Dummy>()));
	}

//	duties_manager_->waitForRequest(avl_handler_);

	tree_manager_->createLocalRootConnection(init_handler_->getEndpointMap(), endpoint_);

}

void Server::getHello() {

	LOG(INFO) << __FUNCTION__;

	tree_manager_->createLocalRootConnection(hello_handler_->getEndpointMap(), endpoint_);

}

