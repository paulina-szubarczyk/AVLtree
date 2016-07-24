#ifndef APP_NETWORK_ROOTMESSAGEHANDLER_H_
#define APP_NETWORK_ROOTMESSAGEHANDLER_H_

#include <network/MessageHandler.h>
#include <network/MessageObserver.h>
#include <tree/AVLTree.h>

class RootMessageHandler: public MessageHandler<sr::MessageToRoot>, public MessageObserver {
public:
	RootMessageHandler(std::shared_ptr<AVLTree> avl_tree);
	virtual ~RootMessageHandler();

	void parseMessage(std::vector<char>& buf);

	bool ready();

private:
	std::shared_ptr<AVLTree> avl_tree_;
//	std::shared_ptr<>
};

#endif /* APP_NETWORK_ROOTMESSAGEHANDLER_H_ */
