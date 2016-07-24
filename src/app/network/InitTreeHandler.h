#ifndef APP_NETWORK_INITTREEHANDLER_H_
#define APP_NETWORK_INITTREEHANDLER_H_

#include "protocol.pb.h"
#include <vector>
#include <map>

#include <network/MessageHandler.h>
#include <network/MessageObserver.h>

class InitTreeHandler : public MessageHandler<sr::InitSystemMessage>, public MessageObserver {

public:
	typedef std::shared_ptr<sr::Tree> TreePtr;
	typedef std::shared_ptr<sr::Hello> HelloPtr;
	typedef std::shared_ptr<sr::Dummy> DummyPtr;

	InitTreeHandler(MessagePtr tree, const int node_counter);
	virtual ~InitTreeHandler() {};

	HelloPtr getHelloMsg() const {
		return hello_msg;
	}

	void parseMessage(std::vector<char>& buf);

	int getNodeCounter() {
		return node_counter_;
	}

	bool ready();

private:
	HelloPtr hello_msg;
	int ip_;
	int node_counter_;

	void addInitToTree(MessagePtr init_msg);

};

#endif /* APP_NETWORK_INITTREEHANDLER_H_ */
