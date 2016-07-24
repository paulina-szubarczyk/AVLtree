#include <network/Sender.h>

Sender::Sender() {
	// TODO Auto-generated constructor stub
	handler_ = std::make_shared<SenderMessageHandler>();
}

Sender::~Sender() {
	// TODO Auto-generated destructor stub
}

void Sender::write(operation fun, std::string word) {
	std::shared_ptr<sr::MessageToRoot> msg = std::make_shared<sr::MessageToRoot>();
	msg->set_word(word);
	msg->set_type(fun == operation::ADD ? sr::MessageToRoot::AddWord : sr::MessageToRoot::RemoveWord);

	connection_->write(msg);

	connection_->readMessage(std::dynamic_pointer_cast<MessageHandler<sr::MessageFromRoot> >(handler_));

}
