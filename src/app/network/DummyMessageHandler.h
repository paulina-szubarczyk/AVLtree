#ifndef APP_NETWORK_DUMMYMESSAGEHANDLER_H_
#define APP_NETWORK_DUMMYMESSAGEHANDLER_H_

#include <network/MessageHandler.h>
#include <network/MessageObserver.h>

class DummyMessageHandler: public MessageHandler<sr::Dummy>, public MessageObserver {
public:
	DummyMessageHandler();
	virtual ~DummyMessageHandler();

	void setCounter(int counter);

	void parseMessage(std::vector<char>& buf);

	bool ready();

private:

	int counter_;

	int index_;
};

#endif /* APP_NETWORK_DUMMYMESSAGEHANDLER_H_ */
