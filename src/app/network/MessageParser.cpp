#include <network/MessageParser.h>
#include <iostream>
#include "glog/logging.h"

template<class MsgType, class BuffType>
const unsigned MessageParser<MsgType, BuffType>::HEADER_SIZE = 4;

template<class Message, class Buffer>
inline MessageParser<Message, Buffer>::MessageParser()
	: message_(new Message()) {}


template<class Message, class Buffer>
inline MessageParser<Message, Buffer>::MessageParser(MessageParser::MsgPtr msg) {
	if(msg == nullptr)
		throw new std::logic_error("Cannot pack a null message");
	message_ = std::move(msg);
}
template<class Message, class Buffer>
inline void MessageParser<Message, Buffer>::resetMsg() {
//	LOG(INFO) << __FUNCTION__ ;

	message_ = nullptr;
	message_ = std::make_shared<Message>();
}

template<class Message, class Buffer>
inline void MessageParser<Message, Buffer>::parseMsg(Buffer& buffer) {

//	LOG(INFO) << __FUNCTION__ ;

	int msg_size = message_->ByteSize();

	LOG(INFO) << "msg_size: " << msg_size;
	buffer.resize(HEADER_SIZE + msg_size);
	encodeHeader(buffer, msg_size);

	message_->SerializeToArray(&buffer[HEADER_SIZE], msg_size);

	std::copy(buffer.begin() + HEADER_SIZE, buffer.end(), std::ostream_iterator<char>(std::cout));
	std::cout << std::endl;
}

template<class Message, class Buffer>
inline bool MessageParser<Message, Buffer>::packMsg(Buffer& buffer) {
//	LOG(INFO) << __FUNCTION__ ;

	std::copy(buffer.begin() + HEADER_SIZE, buffer.end(), std::ostream_iterator<char>(std::cout));
	std::cout << std::endl;

	return message_->ParseFromArray(&buffer[0], buffer.size());
}

template<class Message, class Buffer>
inline unsigned MessageParser<Message, Buffer>::decodeHeader(
		const Buffer& buf) const {
	if (buf.size() < HEADER_SIZE)
		throw new std::logic_error("Buffer's size should be >= 4");

	unsigned msg_size = 0;
	for (unsigned i = 0; i < HEADER_SIZE; ++i)
		msg_size = msg_size * 256 + (static_cast<unsigned>(buf[i]) & 0xFF);
	return msg_size;
}

template<class Message, class Buffer>
inline void MessageParser<Message, Buffer>::encodeHeader(Buffer& buf, int size) {
	assert(buf.size() >= HEADER_SIZE);
	buf[0] = static_cast<uint8_t>((size >> 24) & 0xFF);
	buf[1] = static_cast<uint8_t>((size >> 16) & 0xFF);
	buf[2] = static_cast<uint8_t>((size >> 8) & 0xFF);
	buf[3] = static_cast<uint8_t>(size & 0xFF);
}

template class MessageParser<sr::Tree> ;
template class MessageParser<sr::Hello> ;
template class MessageParser<sr::Message> ;
template class MessageParser<sr::Dummy> ;
template class MessageParser<sr::Response> ;
template class MessageParser<sr::MessageToRoot> ;
template class MessageParser<sr::MessageFromRoot> ;
template class MessageParser<sr::InitSystemMessage> ;


