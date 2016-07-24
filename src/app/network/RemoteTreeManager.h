#ifndef APP_NETWORK_REMOTETREEMANAGER_H_
#define APP_NETWORK_REMOTETREEMANAGER_H_

#include <map>
#include <vector>
#include <network/Connection.h>
#include <network/Endpoint.h>
#include <network/RemoteAVLTreeClient.h>
#include <network/SubtreeMessageParser.h>
#include <tree/AVLTree.h>
#include "protocol.pb.h"

class AVLMessageHandler;
class AVLResponseHandler;

class RemoteTreeManager : public MessageObserver {

public:
	RemoteTreeManager(  std::shared_ptr<boost::asio::io_service> io_service,
						std::shared_ptr<boost::asio::strand> strand,
						std::shared_ptr<AVLMessageHandler> avl_handler);

	virtual ~RemoteTreeManager();

	void createLocalRootConnection(Endpoint::map& endpoints, Endpoint::asio_endpoint& root_endpoint);

	void addRootConnection(Connection::connection_ptr root_connection) ;

	std::vector<Connection::connection_ptr> getConnections() const {
		return connections_;
	}

	void connect();

	bool ready() {
		return is_ready;
	}

	std::shared_ptr<AVLTree> getAvlTree() {
		return avl_tree_;
	}

	void setData(std::shared_ptr<sr::Message> msg);

	void getWord(std::shared_ptr<sr::Message> msg);
	void getCounter(std::shared_ptr<sr::Message> msg);
	void getHeight(std::shared_ptr<sr::Message> msg);

	void insert(std::shared_ptr<sr::Message> msg);
	void remove(std::shared_ptr<sr::Message> msg);

	void rotate_left(std::shared_ptr<sr::Message> msg);
	void rotate_right(std::shared_ptr<sr::Message> msg);

	void push_left_subtree(std::shared_ptr<sr::Message> msg);
	void push_right_subtree(std::shared_ptr<sr::Message> msg);

	void pull_left_subtree(std::shared_ptr<sr::Message> msg);
	void pull_right_subtree(std::shared_ptr<sr::Message> msg);

	void set_subtree(std::shared_ptr<sr::Message> msg);
	void set_left_subtree(std::shared_ptr<sr::Message> msg);
	void set_right_subtree(std::shared_ptr<sr::Message> msg);

	void get_subtree(std::shared_ptr<sr::Message> msg);
	void find_successor(std::shared_ptr<sr::Message> msg);

	void clear(std::shared_ptr<sr::Message> msg) ;

	void preorder(std::shared_ptr<sr::Message> msg) ;

	void notify();

private:
	RemoteAVLTreeClient::client_ptr root_connection_;
	std::vector<Connection::connection_ptr> connections_;
	std::vector<RemoteAVLTreeClient::client_ptr> tree_connections_;

	std::shared_ptr<boost::asio::io_service> io_service_;
	std::shared_ptr<boost::asio::strand> strand_;

	std::shared_ptr<AVLMessageHandler> avl_handler_;
	std::shared_ptr<AVLResponseHandler> avl_response_handler_;
	std::shared_ptr<AVLTree> avl_tree_;

	SubtreeMessageParser subtree_parser_;
	bool is_ready;

	void addConnection(Endpoint::map_endpoint& node) ;
	RemoteAVLTreeClient::client_ptr createChild(Endpoint::map_endpoint& node);

	void registerMessageHandler();

	std::shared_ptr<sr::Response> createResponse(sr::Message::Type type);
	void send(std::shared_ptr<sr::Response> response);

};

#endif /* APP_NETWORK_REMOTETREEMANAGER_H_ */
