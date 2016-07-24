#ifndef APP_NETWORK_MESSAGEPARSER_H_
#define APP_NETWORK_MESSAGEPARSER_H_
#include <memory>
#include "protocol.pb.h"

template<class Message, class Buffer = std::vector<char>> class MessageParser {

public:
	typedef std::shared_ptr<Message> MsgPtr;

	MessageParser();
	MessageParser(MsgPtr msg);
	void parseMsg(Buffer& buffer);
	bool packMsg(Buffer& buffer);

	MsgPtr getMessage() const {
		return message_;
	}

	static const unsigned HEADER_SIZE;

	unsigned decodeHeader(const Buffer& buf) const;

	void resetMsg();
private:
	MsgPtr message_;

	void encodeHeader(Buffer& buf, int size);

};


#endif /* APP_NETWORK_MESSAGEPARSER_H_ */
