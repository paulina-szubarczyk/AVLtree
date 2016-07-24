#ifndef APP_NETWORK_AVLMESSAGEHANDLER_H_
#define APP_NETWORK_AVLMESSAGEHANDLER_H_
#include <network/MessageHandler.h>
#include <network/RemoteTreeManager.h>

class AVLMessageHandler : public MessageHandler<sr::Message> {
public:
	AVLMessageHandler();
	virtual ~AVLMessageHandler();

	typedef boost::function<void (std::shared_ptr<sr::Message>) > handler;
	void parseMessage(std::vector<char>& buf) ;

	bool ready() {
		return true;
	}

	void setTree(std::shared_ptr<RemoteTreeManager> tree) {
		tree_manager_ = tree;
	}

	void registerHandler(sr::Message::Type type, handler fun) {
		message_handlers_[type] = fun;
	}
private:
	std::shared_ptr<RemoteTreeManager> tree_manager_;
	std::map<sr::Message::Type, handler> message_handlers_;
};

#endif /* APP_NETWORK_AVLMESSAGEHANDLER_H_ */
