#ifndef APP_NETWORK_THREADPOOL_H_
#define APP_NETWORK_THREADPOOL_H_
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <memory>

class ThreadPool {
public:

	typedef boost::function< void() > Function;
	ThreadPool( std::shared_ptr<boost::asio::io_service> io_service,
				std::shared_ptr<boost::asio::strand> strand,
				int pool_size);

	virtual ~ThreadPool();

	void invoke(Function fun);

	void waitForFinish();

private:
	std::shared_ptr<boost::asio::io_service> io_service_;
	boost::asio::io_service::work work_;
	std::shared_ptr<boost::asio::strand> strand_;
	boost::thread_group pool_;
	int size_;
};
#endif /* APP_NETWORK_THREADPOOL_H_ */
