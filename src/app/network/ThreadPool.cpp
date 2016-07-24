#include <network/ThreadPool.h>
#include "glog/logging.h"

ThreadPool::ThreadPool( std::shared_ptr<boost::asio::io_service> io_service,
						std::shared_ptr<boost::asio::strand> strand,
						int pool_size)
	: io_service_(io_service), work_(*io_service), strand_(strand), size_(pool_size) {
	LOG(INFO) << __FUNCTION__;

	for (int i=0; i<size_; ++i) {
		pool_.create_thread(boost::bind(&boost::asio::io_service::run, io_service_));
	}
}

ThreadPool::~ThreadPool() {
	try {
		pool_.join_all();
	} catch (...) {
		LOG(WARNING) << "Into thread pool destructor";
	}
}

void ThreadPool::invoke(Function fun) {
	LOG(INFO) << __FUNCTION__;
	io_service_->post(fun);
}

void ThreadPool::waitForFinish() {
	LOG(INFO) << __FUNCTION__;

	pool_.join_all();
}
