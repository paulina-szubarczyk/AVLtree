#include <network/RootDutiesManager.h>

#include "glog/logging.h"

RootDutiesManager::RootDutiesManager(
		std::shared_ptr<boost::asio::io_service> io_service,
		std::shared_ptr<boost::asio::strand> strand, int nodes_counter)
		: io_service_(io_service), strand_(strand) {

	thread_pool_ = std::make_shared<ThreadPool>(io_service, strand_, nodes_counter+1);
}

RootDutiesManager::~RootDutiesManager() {
	// TODO Auto-generated destructor stub
}


void RootDutiesManager::waitForRequest(std::shared_ptr<AVLMessageHandler> avl_handler)
{
	LOG(INFO) << __FUNCTION__;

	avl_handler_ = avl_handler;

	for (auto new_connection : connections_mind_) {

		thread_pool_->invoke(boost::bind(&Connection::readMessage<sr::Message>, new_connection,
							 std::dynamic_pointer_cast<MessageHandler<sr::Message> >(avl_handler_)));
	}
}

void RootDutiesManager::waitForDummy(std::shared_ptr<DummyMessageHandler> dummy_handler)
{
	LOG(INFO) << __FUNCTION__;

	dummy_handler_ = dummy_handler;

	dummy_handler_->setCounter(connections_.size());

	for (auto new_connection : connections_) {

		thread_pool_->invoke(boost::bind(&Connection::readMessage<sr::Dummy>, new_connection,
							 std::dynamic_pointer_cast<MessageHandler<sr::Dummy> >(dummy_handler_)));
	}
}

void RootDutiesManager::heartbeat() {}

void RootDutiesManager::createConnection(Endpoint::map& endpoints, Endpoint::asio_endpoint& root_endpoint)
{

	LOG(INFO) << __FUNCTION__;

	Endpoint::map_endpoint root = Endpoint::resolver(root_endpoint);

	for(auto node :  endpoints) {

		if (node.second != root) {

			Endpoint::asio_endpoint endpoint = Endpoint::resolver(node.second);

			Connection::connection_ptr new_connection = Connection::create(io_service_);

			connections_mind_.push_back(new_connection);

			new_connection->connect(endpoint);
		}
	}
	requests();
}

void RootDutiesManager::requests() {

	for (auto new_connection : connections_mind_) {

		thread_pool_->invoke(boost::bind(&RootDutiesManager::handleRequests, *this, new_connection));
	}
}

void RootDutiesManager::handleRequests(Connection::connection_ptr connection) {
	connection->readMessage<sr::MessageToRoot>(root_handler_);
	std::shared_ptr<sr::MessageFromRoot> msg = std::make_shared<sr::MessageFromRoot>();
	msg->set_reset(false);
	connection->write(msg);
	handleRequests(connection);
}

