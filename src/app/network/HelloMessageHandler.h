#ifndef APP_NETWORK_HELLOMESSAGEHANDLER_H_
#define APP_NETWORK_HELLOMESSAGEHANDLER_H_

#include <network/MessageHandler.h>
#include <network/MessageObserver.h>

#include <map>

class HelloMessageHandler : public MessageHandler<sr::Hello>, public MessageObserver {
public:

	typedef std::shared_ptr<sr::Hello> HelloPtr;

	HelloMessageHandler();

	virtual ~HelloMessageHandler();

	void parseMessage(std::vector<char>& buf);

	bool ready() {
		return is_ready;
	}

private:
	bool is_ready;

};

#endif /* APP_NETWORK_HELLOMESSAGEHANDLER_H_ */
