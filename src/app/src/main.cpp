#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <thread>
#include <memory>
#include <unistd.h>

#include <network/Server.h>
#include <network/InitTreeHandler.h>
#include <network/HelloMessageHandler.h>
#include <network/AVLMessageHandler.h>
#include <network/DummyMessageHandler.h>
#include <src/FileManager.h>

#include <network/Sender.h>

#include "protocol.pb.h"

#include "glog/logging.h"

using boost::asio::ip::tcp;

int main( int argc, char * argv[] )
{

	if (argc < 4) {
		LOG(FATAL) << "not enough value";
		return 1;
	}

	bool is_root = (argc == 4 || argc == 5) ;

	std::string ip(argv[1]);

	int port = atoi(argv[2]);

	std::string file;

	bool file_read = false;

	if (argc == 5) {
		file = std::string(argv[4]);
		file_read = true;
	} else if (argc == 7) {
		file = std::string(argv[6]);
		file_read = true;
	}

	int nodes = (is_root ? atoi(argv[3]) : atoi(argv[5]));



	std::shared_ptr< boost::asio::io_service > io_service = std::make_shared<boost::asio::io_service>();

	std::shared_ptr<Server> server;



	std::shared_ptr<sr::InitSystemMessage> init = std::make_shared<sr::InitSystemMessage>();

	LOG(INFO) << ip;

	init->set_ip(ip);

	init->set_port(port);

	try
	{
		tcp::resolver resolver(*io_service);

		LOG(INFO) << "server endpoint";

		auto endpoint_server = resolver.resolve({argv[1], argv[2]});

		LOG(INFO) << "server create";


		std::shared_ptr<InitTreeHandler> init_handler = std::make_shared<InitTreeHandler>(init, nodes);

		std::shared_ptr<HelloMessageHandler> hello_handler = std::make_shared<HelloMessageHandler>();

		std::shared_ptr<AVLMessageHandler> avl_handler = std::make_shared<AVLMessageHandler>();

		std::shared_ptr<DummyMessageHandler> dummy_handler = std::make_shared<DummyMessageHandler>();

		std::shared_ptr<Sender> sender = std::make_shared<Sender> ();

		server = std::make_shared<Server>(io_service, *endpoint_server, init_handler, hello_handler, avl_handler, dummy_handler, sender, is_root);

		std::shared_ptr<RemoteTreeManager> tree_manager_ = server->getTreeManager();

		std::shared_ptr<FileManager> file_manager = std::make_shared<FileManager>(tree_manager_->getAvlTree(), sender, is_root);

		if (file_read) {

			file_manager->set_file(file);
		}

		tree_manager_->notifyWhenReady(boost::bind(&FileManager::start, file_manager));

		server->start();

		Connection::connection_ptr client;
		if (!is_root) {

			client = std::make_shared<Connection>(io_service);

			tcp::resolver resolver(*io_service);

			auto endpoint_client = resolver.resolve({argv[3], argv[4]});

			client->connect(*endpoint_client);

			client->write(init);

			client->readMessage(std::dynamic_pointer_cast<MessageHandler<sr::Hello> >(hello_handler));

			std::shared_ptr<sr::Dummy> dummy = std::make_shared<sr::Dummy>();

			client->write(dummy);

			client->readMessage(std::dynamic_pointer_cast<MessageHandler<sr::Dummy> >(dummy_handler));

			std::cout << "Read dummy " <<  std::endl;
//			client.close();
		}

		io_service->run();
	}
	catch( std::exception & ex )
	{
		std::cout << "Exception: " << ex.what() << std::endl;
	}




//	io_service->stop();

	return 0;
}

