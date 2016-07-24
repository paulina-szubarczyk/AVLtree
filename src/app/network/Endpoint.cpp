#include <network/Endpoint.h>

Endpoint::map_endpoint Endpoint::resolver(asio_endpoint endpoint) {
	return std::make_pair<std::string, std::string>(endpoint.address().to_string(), std::to_string(endpoint.port()));

}

Endpoint::asio_endpoint Endpoint::resolver(map_endpoint node) {
	return Endpoint::asio_endpoint(asio_address::from_string(node.first), atoi(node.second.c_str()));;
}
