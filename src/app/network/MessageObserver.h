#ifndef APP_NETWORK_MESSAGEOBSERVER_H_
#define APP_NETWORK_MESSAGEOBSERVER_H_


#include <boost/signals2/signal.hpp>
#include <network/Endpoint.h>

class MessageObserver {

public:
	MessageObserver();
	virtual ~MessageObserver();

	typedef boost::signals2::signal<void (void)> nodes_ready;

	void notifyWhenReady(const nodes_ready::slot_type& slot) {
		ready_.connect(slot);
	}

	virtual bool ready() = 0;

	Endpoint::map& getEndpointMap() {
		return endpoint_map_;
	}

protected:
	nodes_ready ready_;
	Endpoint::map endpoint_map_;
};

#endif /* APP_NETWORK_MESSAGEOBSERVER_H_ */
