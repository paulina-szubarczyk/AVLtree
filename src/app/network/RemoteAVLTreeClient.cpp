#include <network/RemoteAVLTreeClient.h>
#include <network/AVLResponseHandler.h>
#include "glog/logging.h"

RemoteAVLTreeClient::RemoteAVLTreeClient(Connection::connection_ptr connection, Endpoint::asio_endpoint& node,
		std::shared_ptr<AVLResponseHandler> avl_handler)
		: connection_(connection), node_(node), avl_handler_(avl_handler) {
	LOG(INFO) << __FUNCTION__;

};

RemoteAVLTreeClient::client_ptr RemoteAVLTreeClient::create(Connection::connection_ptr connection, Endpoint::asio_endpoint node,
		std::shared_ptr<AVLResponseHandler> avl_handler)	{
	LOG(INFO) << __FUNCTION__;

	return std::make_shared<RemoteAVLTreeClient>(connection, node, avl_handler);
}

RemoteAVLTreeClient::~RemoteAVLTreeClient() {
	LOG(INFO) << __FUNCTION__;
}

RemoteAVLTreeClient::message_ptr RemoteAVLTreeClient::createMessage(sr::Message::Type type) {
	LOG(INFO) << __FUNCTION__;

	message_ptr msg = std::make_shared<sr::Message>();
	msg->set_type(type);
	return msg;
}

RemoteAVLTreeClient::response_ptr RemoteAVLTreeClient::handle_response(sr::Message::Type type) {
	response_ptr response = avl_handler_->getMessage();
	if(response->has_type() && response->type() == type) {
		return response;
	}
	//TODO logic error
	LOG(WARNING) << __FUNCTION__ << " Bad response ";
	return nullptr;
}

void RemoteAVLTreeClient::setData(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::SetData;
	message_ptr msg = createMessage(type);
	sr::DataNodeMessage* node = msg->mutable_datanode();
	node->set_word(word);
	node->set_counter(counter);

	send(msg);

	handle_response(type);
}

std::string RemoteAVLTreeClient::getWord() {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::GetValue;
	message_ptr msg = createMessage(type);
	send(msg);

	response_ptr response = handle_response(type);
	if(response) {
		return response->has_stringvalue() ? response->stringvalue() : std::string();
	}
	return 0;
}

int RemoteAVLTreeClient::getCounter() {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::GetCount;
	message_ptr msg = createMessage(type);
	send(msg);
	response_ptr response = handle_response(type);
	if(response) {
		return response->has_intvalue() ? response->intvalue() : 0;
	}
	return 0;
}

int RemoteAVLTreeClient::getHeight() {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::GetHeight;
	message_ptr msg = createMessage(type);
	send(msg);
	response_ptr response = handle_response(type);
	if(response) {
		return response->has_intvalue() ? response->intvalue() : 0;
	}
	return 0;
}


void RemoteAVLTreeClient::insert(std::string word) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::Insert;
	message_ptr msg = createMessage(type);
	sr::Insert* ins = msg->mutable_insert();
	ins->set_word(word);

	send(msg);
	handle_response(type);
}

void RemoteAVLTreeClient::remove(std::string word) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::Remove;
	message_ptr msg = createMessage(type);
	sr::Remove* remove = msg->mutable_remove();
	remove->set_word(word);

	send(msg);
	handle_response(type);
}

RemoteAVLTreeClient::ProtoTreePtr RemoteAVLTreeClient::get_subtree(sr::Message::Branch branch) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::GetSubtree;
	message_ptr msg = createMessage(type);
	msg->set_branch(branch);
	send(msg);
	response_ptr response = handle_response(type);
	if(response) {
		return response->has_subtree() ?
				std::make_shared<sr::PabloSubtree>(response->subtree()) : nullptr;
	}
	return nullptr;
}

RemoteAVLTreeClient::ProtoNodePtr RemoteAVLTreeClient::find_successor(){
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::FindLesserSuccessor;
	message_ptr msg = createMessage(type);
	send(msg);
	response_ptr response = handle_response(type);
	if(response) {
		return response->has_datanode() ?
				std::make_shared<sr::DataNodeMessage>(response->datanode()) : nullptr;
	}
	return nullptr;
}

void RemoteAVLTreeClient::push_left_subtree(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::PushLeftSubTree;

	message_ptr msg = createMessage(type);
	sr::DataNodeMessage* node = msg->mutable_datanode();
	node->set_word(word);
	node->set_counter(counter);

	send(msg);

	handle_response(type);
}

void RemoteAVLTreeClient::push_right_subtree(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::PushRightSubTree;

	message_ptr msg = createMessage(type);
	sr::DataNodeMessage* node = msg->mutable_datanode();
	node->set_word(word);
	node->set_counter(counter);

	send(msg);
	handle_response(type);
}

void RemoteAVLTreeClient::pull_left_subtree() {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::PullLeftSubTree;

	message_ptr msg = createMessage(type);
	send(msg);
	handle_response(type);
}

void RemoteAVLTreeClient::pull_right_subtree() {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::PullLeftSubTree;

	message_ptr msg = createMessage(type);
	send(msg);
	handle_response(type);
}

void RemoteAVLTreeClient::set_subtree(ProtoTreePtr tree) {
	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::SetSubTree;

	message_ptr msg = createMessage(type);

    sr::PabloSubtree* proto_tree = msg->mutable_subtree();
    *proto_tree = *tree;

	send(msg);
	handle_response(type);
}

void RemoteAVLTreeClient::set_left_subtree(ProtoTreePtr tree) {

	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::SetLeftSubTree;

	message_ptr msg = createMessage(type);

	if (tree) {
	    sr::PabloSubtree* proto_tree = msg->mutable_subtree();
	    *proto_tree = *tree;
	} else {
		LOG(WARNING) << "Empty tree ";
	}

	send(msg);

	handle_response(type);
}

void RemoteAVLTreeClient::set_right_subtree(ProtoTreePtr tree) {

	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::SetRightSubTree;

	message_ptr msg = createMessage(type);

    sr::PabloSubtree* proto_tree = msg->mutable_subtree();
    *proto_tree = *tree;

	send(msg);

	handle_response(type);
}

void RemoteAVLTreeClient::rotate_left() {

	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::RotateWithLeftChild;

	message_ptr msg = createMessage(type);

	send(msg);

	handle_response(type);
}

void RemoteAVLTreeClient::rotate_right() {

	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::RotateWithRightChild;

	message_ptr msg = createMessage(type);

	send(msg);

	handle_response(type);
}

void RemoteAVLTreeClient::clear() {

	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::Clear2;

	message_ptr msg = createMessage(type);

	send(msg);

	handle_response(type);
}


void RemoteAVLTreeClient::preorder() {

	LOG(INFO) << __FUNCTION__;

	sr::Message::Type type = sr::Message::Preorder;

	message_ptr msg = createMessage(type);

	send(msg);

	handle_response(type);
}

void RemoteAVLTreeClient::send(message_ptr msg) {

	LOG(INFO) << __FUNCTION__;

	connection_->write(msg);

	connection_->readMessage(std::dynamic_pointer_cast<MessageHandler<sr::Response> >(avl_handler_));
}




