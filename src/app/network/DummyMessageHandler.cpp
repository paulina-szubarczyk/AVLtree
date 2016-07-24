#include <network/DummyMessageHandler.h>
#include "glog/logging.h"

DummyMessageHandler::DummyMessageHandler() {
	// TODO Auto-generated constructor stub

}

DummyMessageHandler::~DummyMessageHandler() {
	// TODO Auto-generated destructor stub
}

void DummyMessageHandler::parseMessage(std::vector<char>& buf) {

	LOG(INFO) << __FUNCTION__;

	++index_;

	parser_.resetMsg();

	if (parser_.packMsg(buf)) {
		if (ready())
			ready_();
	}
	else
		LOG(WARNING) << "Error during parsing";
}

bool DummyMessageHandler::ready() {
	LOG(INFO) << __FUNCTION__ << index_ << " " << counter_;

	return index_ == counter_;
}

void DummyMessageHandler::setCounter(int counter) {
	counter_ = counter;
}
