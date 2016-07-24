#ifndef APP_NETWORK_SENDERMESSAGEHANDLER_H_
#define APP_NETWORK_SENDERMESSAGEHANDLER_H_

#include <network/MessageHandler.h>

class SenderMessageHandler: public MessageHandler<sr::MessageFromRoot> {
public:
	SenderMessageHandler();
	virtual ~SenderMessageHandler();

	void parseMessage(std::vector<char>& buf);

	bool ready();
};

#endif /* APP_NETWORK_SENDERMESSAGEHANDLER_H_ */
