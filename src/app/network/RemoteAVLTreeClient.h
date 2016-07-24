#ifndef APP_NETWORK_REMOTEAVLTREECLIENT_H_
#define APP_NETWORK_REMOTEAVLTREECLIENT_H_
#include <network/Connection.h>
#include <network/Endpoint.h>
#include "protocol.pb.h"
#include <memory>

class AVLResponseHandler;
class RemoteAVLTreeClient  {
public:

	typedef std::shared_ptr<RemoteAVLTreeClient> client_ptr;
	typedef std::shared_ptr<sr::Message> message_ptr;
	typedef std::shared_ptr<sr::Response> response_ptr;
	typedef std::shared_ptr<sr::DataNodeMessage> ProtoNodePtr;
	typedef std::shared_ptr<sr::PabloSubtree> ProtoTreePtr;



	RemoteAVLTreeClient(Connection::connection_ptr connection, Endpoint::asio_endpoint& node,
			std::shared_ptr<AVLResponseHandler> avl_handler);

	virtual ~RemoteAVLTreeClient();

	static client_ptr create(Connection::connection_ptr connection,
			Endpoint::asio_endpoint node,
			std::shared_ptr<AVLResponseHandler> avl_handler)	;

	void connect() {
		connection_->connect(node_);
	}

	const Endpoint::asio_endpoint& getNode() const {
		return node_;
	}

	void setData(std::string word, int counter);

	std::string getWord();
	int getCounter();
	int getHeight();

	void insert(std::string word);
	void remove(std::string word);

	void rotate_left();
	void rotate_right();

	void push_left_subtree(std::string word, int counter);
	void push_right_subtree(std::string word, int counter);

	void pull_left_subtree();
	void pull_right_subtree();

	void set_subtree(ProtoTreePtr tree);
	void set_left_subtree(ProtoTreePtr tree);
	void set_right_subtree(ProtoTreePtr tree);

	ProtoTreePtr get_subtree(sr::Message::Branch branch);

	ProtoNodePtr find_successor();

	void clear() ;

	void preorder();

	Connection::connection_ptr getConnection() const {
		return connection_;
	}

private:
	Connection::connection_ptr connection_;
	Endpoint::asio_endpoint node_;
	std::shared_ptr<AVLResponseHandler> avl_handler_;

	void send(message_ptr msg);
	message_ptr createMessage(sr::Message::Type type);
	response_ptr handle_response(sr::Message::Type type);
};

#endif /* APP_NETWORK_REMOTEAVLTREECLIENT_H_ */
