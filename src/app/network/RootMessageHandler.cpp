#include <network/RootMessageHandler.h>

RootMessageHandler::RootMessageHandler(std::shared_ptr<AVLTree> avl_tree) : avl_tree_(avl_tree)
{
	// TODO Auto-generated constructor stub

}

RootMessageHandler::~RootMessageHandler() {
	// TODO Auto-generated destructor stub
}

void RootMessageHandler::parseMessage(std::vector<char>& buf) {
	LOG(INFO) << __FUNCTION__;


	parser_.resetMsg();

	if (parser_.packMsg(buf)) {
		std::shared_ptr<sr::MessageToRoot> msg = parser_.getMessage();

		switch(msg->type()) {
		case sr::MessageToRoot::AddWord:
			avl_tree_->insert(msg->word());
			break;
		case sr::MessageToRoot::RemoveWord:
			avl_tree_->remove(msg->word());
			break;
		case sr::MessageToRoot::Heartbeat:

			break;
		}
		ready_();
	}
	else
		LOG(WARNING) << "Error during parsing";
}

bool RootMessageHandler::ready() {

}
