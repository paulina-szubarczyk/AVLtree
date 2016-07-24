#include <tree/AVLTree.h>
#include <boost/thread.hpp>
#include "glog/logging.h"

AVLTree::AVLTree() : root_(nullptr), queue_(10000)
{
	LOG(INFO) << __FUNCTION__;

}

AVLTree::~AVLTree() {
	LOG(INFO) << __FUNCTION__;

}

void AVLTree::run() {
	LOG(INFO) << __FUNCTION__ << " " ;

	boost::thread thread(boost::bind(&AVLTree::invoke,this));
}

void AVLTree::setRoot(NodeData::NodePtr root) {
	LOG(INFO) << __FUNCTION__;

	root_ = root;
}

NodeData::NodePtr AVLTree::getRoot() {
	LOG(INFO) << __FUNCTION__;

	return root_;
}

void AVLTree::insert(std::string word) {
	LOG(INFO) << __FUNCTION__ << " " <<  word;

	words_.push_back(word);
	operation op = operation::ADD;
	queue_.push(op);
}

void AVLTree::remove(std::string word) {
	LOG(INFO) << __FUNCTION__ << " " <<  word;

	words_.push_back(word);
	operation op = operation::REMOVE;
	queue_.push(op);
}

void AVLTree::preorder() {
	LOG(INFO) << __FUNCTION__ << " " ;

	root_->preorder();
}

void AVLTree::invoke() {
	LOG(INFO) << __FUNCTION__ << " " ;

	operation oper;
	int count = 0;
	while (true) {
		if(queue_.pop(oper)) {
			switch(oper) {
			case ADD :
				root_->insert(words_.back());
				root_->preorder();
				break;
			case REMOVE :
				root_->remove(words_.back());
				root_->preorder();
				break;
			}
			words_.pop_back();
			count = 1;
		} else if (count) {
			root_->preorder();
			count = 0;
		}
	}
}

