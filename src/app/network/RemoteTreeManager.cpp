#include <network/RemoteTreeManager.h>
#include <network/AVLMessageHandler.h>
#include <network/AVLResponseHandler.h>
#include <tree/AVLTree.h>
#include <tree/RemoteDataNode.h>
#include <tree/NodeData.h>
#include "glog/logging.h"
#include "protocol.pb.h"

RemoteTreeManager::RemoteTreeManager(
		std::shared_ptr<boost::asio::io_service> io_service,
		std::shared_ptr<boost::asio::strand> strand,
		std::shared_ptr<AVLMessageHandler> avl_handler)
		: io_service_(io_service), strand_(strand), avl_handler_(avl_handler)
{
	LOG(INFO) << __FUNCTION__;

	avl_tree_ = std::make_shared<AVLTree>();
	avl_response_handler_ = std::make_shared<AVLResponseHandler>();
	registerMessageHandler();
}

RemoteTreeManager::~RemoteTreeManager()
{
	LOG(INFO) << __FUNCTION__;
}

void RemoteTreeManager::addRootConnection(Connection::connection_ptr root_connection)
{

	LOG(INFO) << __FUNCTION__;

	if(root_connection) {
		root_connection_ = RemoteAVLTreeClient::create(root_connection,
										root_connection->socket().remote_endpoint(), avl_response_handler_);
	} else {
		root_connection_ = nullptr;
	}
}


void RemoteTreeManager::createLocalRootConnection(Endpoint::map& endpoints, Endpoint::asio_endpoint& root_endpoint)
{
	LOG(INFO) << __FUNCTION__;

	Endpoint::map_endpoint node_ = Endpoint::resolver(root_endpoint);

	Endpoint::map::iterator node = endpoints.begin();

	while (node->second != node_)	{
		++node;
	}

	int parent = node->first/2;
	int left_child = node->first*2;
	int right_child = node->first*2+1;

	NodeData::NodePtr local_root = RemoteDataNode::create(nullptr);

	if (parent > 1 && endpoints.find(parent) != endpoints.end()) {
		local_root->setParent(RemoteDataNode::create(createChild(endpoints[parent])));
	}

	if (endpoints.find(left_child) != endpoints.end()) {
		local_root->setLeft(RemoteDataNode::create(createChild(endpoints[left_child])));
	}

	if (endpoints.find(right_child) != endpoints.end()) {
		local_root->setRight(RemoteDataNode::create(createChild(endpoints[right_child])));
	}

	avl_tree_->setRoot(local_root);

	is_ready = true;

}

void RemoteTreeManager::notify() {

	LOG(INFO) << __FUNCTION__;

	while(!is_ready) {
		sleep(1);
	}

	ready_();
}

RemoteAVLTreeClient::client_ptr RemoteTreeManager::createChild(Endpoint::map_endpoint& node) {

	LOG(INFO) << __FUNCTION__;

	Endpoint::asio_endpoint endpoint = Endpoint::resolver(node);

	Connection::connection_ptr new_connection = Connection::create(io_service_);

	RemoteAVLTreeClient::client_ptr new_client = RemoteAVLTreeClient::create(new_connection, endpoint, avl_response_handler_);

	connections_.push_back(new_connection);

	tree_connections_.push_back(new_client);

	new_client->connect();

	return new_client;
}

void RemoteTreeManager::registerMessageHandler() {
	LOG(INFO) << __FUNCTION__;

	avl_handler_->registerHandler(sr::Message::SetData, boost::bind(&RemoteTreeManager::setData,this,_1));
	avl_handler_->registerHandler(sr::Message::GetValue, boost::bind(&RemoteTreeManager::getWord,this,_1));
	avl_handler_->registerHandler(sr::Message::GetCount, boost::bind(&RemoteTreeManager::getCounter,this,_1));
	avl_handler_->registerHandler(sr::Message::GetHeight, boost::bind(&RemoteTreeManager::getHeight,this,_1));

	avl_handler_->registerHandler(sr::Message::Insert, boost::bind(&RemoteTreeManager::insert,this,_1));
	avl_handler_->registerHandler(sr::Message::Remove, boost::bind(&RemoteTreeManager::remove,this,_1));

	avl_handler_->registerHandler(sr::Message::RotateWithLeftChild, boost::bind(&RemoteTreeManager::rotate_left,this,_1));
	avl_handler_->registerHandler(sr::Message::RotateWithRightChild, boost::bind(&RemoteTreeManager::rotate_right,this,_1));

	avl_handler_->registerHandler(sr::Message::PushLeftSubTree, boost::bind(&RemoteTreeManager::push_left_subtree,this,_1));
	avl_handler_->registerHandler(sr::Message::PushRightSubTree, boost::bind(&RemoteTreeManager::push_right_subtree,this,_1));

	avl_handler_->registerHandler(sr::Message::PullLeftSubTree, boost::bind(&RemoteTreeManager::pull_left_subtree,this,_1));
	avl_handler_->registerHandler(sr::Message::PullRightSubTree, boost::bind(&RemoteTreeManager::pull_right_subtree,this,_1));

	avl_handler_->registerHandler(sr::Message::SetSubTree, boost::bind(&RemoteTreeManager::set_subtree,this,_1));
	avl_handler_->registerHandler(sr::Message::SetLeftSubTree, boost::bind(&RemoteTreeManager::set_left_subtree,this,_1));
	avl_handler_->registerHandler(sr::Message::SetRightSubTree, boost::bind(&RemoteTreeManager::set_right_subtree,this,_1));

	avl_handler_->registerHandler(sr::Message::GetSubtree, boost::bind(&RemoteTreeManager::get_subtree,this,_1));
	avl_handler_->registerHandler(sr::Message::FindLesserSuccessor, boost::bind(&RemoteTreeManager::find_successor,this,_1));

	avl_handler_->registerHandler(sr::Message::Clear2, boost::bind(&RemoteTreeManager::clear,this,_1));
	avl_handler_->registerHandler(sr::Message::Preorder, boost::bind(&RemoteTreeManager::preorder,this,_1));

}

std::shared_ptr<sr::Response> RemoteTreeManager::createResponse(sr::Message::Type type) {

	LOG(INFO) << __FUNCTION__;

	std::shared_ptr<sr::Response> response = std::make_shared<sr::Response>();

	response->set_type(type);

	return response;
}

void RemoteTreeManager::send(std::shared_ptr<sr::Response> response) {

	LOG(INFO) << __FUNCTION__;

	root_connection_->getConnection()->write(response);

	root_connection_->getConnection()->readMessage(std::dynamic_pointer_cast<MessageHandler<sr::Message> >(avl_handler_));
}

void RemoteTreeManager::setData(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->setData(msg->datanode().word(), msg->datanode().counter());

	send(createResponse(sr::Message::SetData));
}

void RemoteTreeManager::getWord(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	auto response = createResponse(sr::Message::GetValue);

	response->set_stringvalue(avl_tree_->getRoot()->getWord());

	send(response);
}

void RemoteTreeManager::getCounter(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	auto response = createResponse(sr::Message::GetCount);

	response->set_intvalue(avl_tree_->getRoot()->getCounter());

	send(response);

}

void RemoteTreeManager::getHeight(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	auto response = createResponse(sr::Message::GetHeight);

	response->set_intvalue(avl_tree_->getRoot()->getHeight());

	send(response);
}

void RemoteTreeManager::insert(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	std::string word = msg->insert().word();

	LOG(INFO) << word;

	avl_tree_->getRoot()->insert(word);

	send(createResponse(sr::Message::Insert));

}

void RemoteTreeManager::remove(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	std::string word = msg->remove().word();

	LOG(INFO) << word;

	avl_tree_->getRoot()->remove(word);

	send(createResponse(sr::Message::Remove));

}

void RemoteTreeManager::rotate_left(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->rotate_left();

	send(createResponse(sr::Message::RotateWithLeftChild));
}

void RemoteTreeManager::rotate_right(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->rotate_right();

	send(createResponse(sr::Message::RotateWithRightChild));

}

void RemoteTreeManager::push_left_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->push_left_subtree(msg->datanode().word(), msg->datanode().counter());

	send(createResponse(sr::Message::PushLeftSubTree));

}

void RemoteTreeManager::push_right_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->push_right_subtree(msg->datanode().word(), msg->datanode().counter());

	send(createResponse(sr::Message::PushRightSubTree));

}

void RemoteTreeManager::pull_left_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->pull_left_subtree();

	send(createResponse(sr::Message::PullLeftSubTree));

}

void RemoteTreeManager::pull_right_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->pull_right_subtree();

	send(createResponse(sr::Message::PullRightSubTree));
}

void RemoteTreeManager::set_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->set_subtree(subtree_parser_.parseTree(msg->subtree()));

	send(createResponse(sr::Message::SetSubTree));

}

void RemoteTreeManager::set_left_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->set_left_subtree(subtree_parser_.parseTree(msg->subtree()));

	send(createResponse(sr::Message::SetLeftSubTree));

}

void RemoteTreeManager::set_right_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->set_right_subtree(subtree_parser_.parseTree(msg->subtree()));

	send(createResponse(sr::Message::SetRightSubTree));

}

void RemoteTreeManager::get_subtree(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	auto response = createResponse(sr::Message::GetSubtree);

	if (msg->branch() == sr::Message::Both) {

		sr::PabloSubtree* tree = response->mutable_subtree();
		*tree = *(subtree_parser_.parseProtoTree(avl_tree_->getRoot()));

	} else if (msg->branch() == sr::Message::Left) {
		sr::PabloSubtree* tree = response->mutable_subtree();
		*tree = *(subtree_parser_.parseProtoTree(avl_tree_->getRoot()->getLeft()));


	} else if (msg->branch() == sr::Message::Right) {
		sr::PabloSubtree* tree = response->mutable_subtree();
		*tree = *(subtree_parser_.parseProtoTree(avl_tree_->getRoot()->getRight()));
	}

	send(response);
}

void RemoteTreeManager::find_successor(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	auto response = createResponse(sr::Message::FindLesserSuccessor);

	NodeData::NodePtr node = avl_tree_->getRoot()->find_successor();
	sr::DataNodeMessage* proto_node = response->mutable_datanode();
	*proto_node = *(subtree_parser_.parseProtoNode(node).get());

	send(response);
}

void RemoteTreeManager::clear(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->clear();

	send(createResponse(sr::Message::Clear2));

}

void RemoteTreeManager::preorder(std::shared_ptr<sr::Message> msg) {

	LOG(INFO) << __FUNCTION__;

	avl_tree_->getRoot()->preorder();

	send(createResponse(sr::Message::Preorder));

}
