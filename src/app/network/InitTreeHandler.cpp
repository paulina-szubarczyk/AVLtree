#include <network/InitTreeHandler.h>
#include "glog/logging.h"
#include <iostream>

InitTreeHandler::InitTreeHandler(MessagePtr root, const int node_counter)
	: MessageHandler<sr::InitSystemMessage>(), ip_(1), node_counter_(node_counter) {

	hello_msg = std::make_shared<sr::Hello>();
	hello_msg->set_id(1);
	addInitToTree(root);
}

void InitTreeHandler::parseMessage(std::vector<char>& buf) {

	parser_.resetMsg();

	if (parser_.packMsg(buf)) {
		LOG(INFO) << "Message parsed \n ip " <<  parser_.getMessage()->ip() << " \n port " << parser_.getMessage()->port();
		addInitToTree(parser_.getMessage());
		if (ready())
			ready_();
	}
	else
		LOG(WARNING) << "Error during parsing";
}

void InitTreeHandler::addInitToTree(MessagePtr init_msg) {

	auto tree = hello_msg->add_tree() ;
	endpoint_map_[ip_] = std::make_pair(init_msg->ip(), std::to_string(init_msg->port()));
	tree->set_id(ip_++);
	tree->set_ip(init_msg->ip());
	tree->set_port(init_msg->port());

	LOG(INFO) << "Added: " << tree->id() << " " << tree->ip() << " " << tree->port() << " " <<hello_msg->tree_size();
 }

bool InitTreeHandler::ready() {
	return ip_ > node_counter_;
}
