#include <network/AVLResponseHandler.h>
#include "glog/logging.h"

AVLResponseHandler::AVLResponseHandler() {
	LOG(INFO) << __FUNCTION__;

}

AVLResponseHandler::~AVLResponseHandler() {
	LOG(INFO) << __FUNCTION__;
}

void AVLResponseHandler::parseMessage(std::vector<char>& buffer) {
	LOG(INFO) << __FUNCTION__;

	parser_.resetMsg();

	if (parser_.packMsg(buffer)) {
		LOG(INFO) << "Message parsed";
	} else {
		LOG(INFO) << "Message error \n";
	}
}
