#include <network/AVLMessageHandler.h>
#include "glog/logging.h"
#include "protocol.pb.h"

AVLMessageHandler::AVLMessageHandler() {
	// TODO Auto-generated constructor stub

}

AVLMessageHandler::~AVLMessageHandler() {
	// TODO Auto-generated destructor stub
}

void AVLMessageHandler::parseMessage(std::vector<char>& buffer) {
	LOG(INFO) << __FUNCTION__;

	parser_.resetMsg();

	if (parser_.packMsg(buffer)) {
		LOG(INFO) << "Message parsed";

		auto handler = message_handlers_.find(parser_.getMessage()->type());

		if (handler != message_handlers_.end()) {
			(handler->second)(parser_.getMessage());
		}

		LOG(INFO) << "Message handler not defined \n";

	} else {
		LOG(INFO) << "Message error \n";
	}
}
