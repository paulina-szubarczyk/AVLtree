#include <network/HelloMessageHandler.h>
#include "glog/logging.h"

HelloMessageHandler::HelloMessageHandler() : is_ready(false) {
	// TODO Auto-generated constructor stub

}

HelloMessageHandler::~HelloMessageHandler() {
	// TODO Auto-generated destructor stub
}


void HelloMessageHandler::parseMessage(std::vector<char>& buffer) {
	LOG(INFO) << __FUNCTION__;

	parser_.resetMsg();

	if (parser_.packMsg(buffer)) {
		LOG(INFO) << "Message parsed";

		auto nodes = parser_.getMessage()->tree();
		for(auto node : nodes) {
			endpoint_map_[node.id()] = std::make_pair(node.ip(),std::to_string(node.port()));
		}
		is_ready = true;
		ready_();

	} else {
		LOG(INFO) << "Message error \n";
	}
}
