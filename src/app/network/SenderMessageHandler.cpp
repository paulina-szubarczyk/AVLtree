#include <network/SenderMessageHandler.h>
#include "glog/logging.h"

SenderMessageHandler::SenderMessageHandler() {
	// TODO Auto-generated constructor stub

}

SenderMessageHandler::~SenderMessageHandler() {
	// TODO Auto-generated destructor stub
}

void SenderMessageHandler::parseMessage(std::vector<char>& buf) {
	parser_.resetMsg();

	if (parser_.packMsg(buf)) {
		std::shared_ptr<sr::MessageFromRoot> msg = parser_.getMessage();
	}
	else
		LOG(WARNING) << "Error during parsing";
}

bool SenderMessageHandler::ready() {
}
