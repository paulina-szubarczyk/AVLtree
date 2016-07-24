#ifndef APP_NETWORK_MESSAGEHANDLER_H_
#define APP_NETWORK_MESSAGEHANDLER_H_

#include <vector>
#include <network/MessageParser.h>

template<class Message> class MessageHandler {
public:
	MessageHandler() {} ;
	virtual ~MessageHandler() {} ;

	typedef std::shared_ptr<Message> MessagePtr;

	virtual void parseMessage(std::vector<char>& buf) = 0;

	virtual bool ready() = 0;

	MessagePtr getMessage() const {
		return parser_.getMessage();
	}
protected:
	MessageParser<Message> parser_;
};

#endif /* APP_NETWORK_MESSAGEHANDLER_H_ */
