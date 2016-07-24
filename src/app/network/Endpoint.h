#ifndef APP_NETWORK_ENDPOINT_H_
#define APP_NETWORK_ENDPOINT_H_

#include <map>
#include <boost/asio.hpp>

class Endpoint {
public:

	typedef int map_id;
	typedef std::pair<std::string, std::string> map_endpoint;
	typedef std::map<map_id, map_endpoint> map;
	typedef boost::asio::ip::tcp::endpoint asio_endpoint;
	typedef boost::asio::ip::address asio_address;

	static map_endpoint resolver(asio_endpoint endpoint);
	static asio_endpoint resolver(map_endpoint);

};

#endif /* APP_NETWORK_ENDPOINT_H_ */
