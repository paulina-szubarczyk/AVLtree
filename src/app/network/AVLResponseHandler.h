#ifndef APP_NETWORK_AVLRESPONSEHANDLER_H_
#define APP_NETWORK_AVLRESPONSEHANDLER_H_

#include <network/MessageHandler.h>
#include "protocol.pb.h"
#include <vector>

class AVLResponseHandler: public MessageHandler<sr::Response> {
public:
	AVLResponseHandler();
	virtual ~AVLResponseHandler();

	void parseMessage(std::vector<char>& buffer) ;

	bool ready() {
		return true;
	}
};

#endif /* APP_NETWORK_AVLRESPONSEHANDLER_H_ */
