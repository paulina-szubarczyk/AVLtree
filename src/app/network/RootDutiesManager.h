#ifndef APP_NETWORK_ROOTDUTIESMANAGER_H_
#define APP_NETWORK_ROOTDUTIESMANAGER_H_

#include <map>
#include <vector>
#include <network/ThreadPool.h>
#include <network/Connection.h>
#include <network/Endpoint.h>
#include <network/AVLMessageHandler.h>
#include <network/DummyMessageHandler.h>
#include <network/RootMessageHandler.h>
#include <network/InitTreeHandler.h>

class RootDutiesManager {
public:

	typedef int EndpointKey;
	typedef std::pair<std::string, std::string> EndpointValue;
	typedef std::map<EndpointKey, EndpointValue> EndpointMap;

	RootDutiesManager( std::shared_ptr<boost::asio::io_service> io_service,
			std::shared_ptr<boost::asio::strand> strand, int nodes_counter);

	virtual ~RootDutiesManager();

	void heartbeat();

	void waitForRequest(std::shared_ptr<AVLMessageHandler> avl_handler) ;

	void waitForDummy(std::shared_ptr<DummyMessageHandler> dummy_handler) ;

	void createConnection(Endpoint::map& endpoints, Endpoint::asio_endpoint& root_endpoint);

	std::shared_ptr<ThreadPool> getThreadPool() const {
		return thread_pool_;
	}

	void add_connection(Connection::connection_ptr client) {
		connections_.push_back(client);
	}
	std::vector<Connection::connection_ptr> getConnections() const {
		return connections_;
	}

	void setRootHandler(std::shared_ptr<RootMessageHandler> root_handler) {
		root_handler_ = root_handler;
	}


private:

	std::vector<Connection::connection_ptr> connections_;
	std::vector<Connection::connection_ptr> connections_mind_;

	std::shared_ptr<ThreadPool> thread_pool_;

	std::shared_ptr<boost::asio::io_service> io_service_;
	std::shared_ptr<AVLMessageHandler> avl_handler_;
	std::shared_ptr<DummyMessageHandler> dummy_handler_;
	std::shared_ptr<RootMessageHandler> root_handler_;

	std::shared_ptr<boost::asio::strand> strand_;

	EndpointMap endpoints_map_;

	void requests();
	void handleRequests(Connection::connection_ptr connection);

};

#endif /* APP_NETWORK_ROOTDUTIESMANAGER_H_ */
