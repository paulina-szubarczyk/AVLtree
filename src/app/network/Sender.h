#ifndef APP_NETWORK_SENDER_H_
#define APP_NETWORK_SENDER_H_
#include <network/Connection.h>
#include <network/SenderMessageHandler.h>

class Sender {
public:
	Sender();
	virtual ~Sender();

	void setConnection(Connection::connection_ptr connection) {
		connection_ = connection;
	}

	enum operation {
		ADD,
		REMOVE
	};

	void write(operation fun, std::string word);

private:

	Connection::connection_ptr connection_;
	std::shared_ptr<SenderMessageHandler> handler_;
};

#endif /* APP_NETWORK_SENDER_H_ */
