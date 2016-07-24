#include <tree/RemoteDataNode.h>
#include "glog/logging.h"

RemoteDataNode::RemoteDataNode(RemoteAVLTreeClient::client_ptr client)
		: client_(client)
{
	LOG(INFO) << __FUNCTION__;
}
RemoteDataNode::~RemoteDataNode() {}

RemoteDataNode::NodePtr RemoteDataNode::create(RemoteAVLTreeClient::client_ptr client) {
	LOG(INFO) << __FUNCTION__;

	return std::make_shared<RemoteDataNode>(client);
}



void RemoteDataNode::insert(std::string word) {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		client_->insert(word);
		return;
	}

	NodeData::insert(word);
}

void RemoteDataNode::remove(std::string word) {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		client_->remove(word);
		return;
	}

	NodeData::remove(word);
}

int RemoteDataNode::getHeight() {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		return client_->getHeight();
	}

	return NodeData::getHeight();
}

void RemoteDataNode::setData(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->setData(word, counter);
		return;
	}

	NodeData::setData(word, counter);
}

std::string RemoteDataNode::getWord() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		return client_->getWord();
	}

	return NodeData::getWord();
}

int RemoteDataNode::getCounter() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		return client_->getCounter();
	}

	return NodeData::getCounter();
}

NodeData::NodePtr RemoteDataNode::get_subtree() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		SubtreeMessageParser::ProtoTreePtr proto_tree = client_->get_subtree(sr::Message::Both);
		return proto_tree ? subtree_parser_.parseTree(*proto_tree.get()) : nullptr;
	}

	return NodeData::get_subtree();
}

NodeData::NodePtr RemoteDataNode::get_right_subtree() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		SubtreeMessageParser::ProtoTreePtr proto_tree = client_->get_subtree(sr::Message::Right);
		return proto_tree ? subtree_parser_.parseTree(*proto_tree.get()) : nullptr;
	}

	return NodeData::get_right_subtree();
}

NodeData::NodePtr RemoteDataNode::get_left_subtree() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		SubtreeMessageParser::ProtoTreePtr proto_tree = client_->get_subtree(sr::Message::Left);
		return proto_tree ? subtree_parser_.parseTree(*proto_tree.get()) : nullptr;
	}

	return NodeData::get_left_subtree();
}

void RemoteDataNode::rotate_left() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->rotate_left();
		return;
	}

	NodeData::rotate_left();
	return;
}

void RemoteDataNode::rotate_right() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->rotate_right();
		return;
	}

	NodeData::rotate_right();
	return;
}

void RemoteDataNode::push_left_subtree(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->push_left_subtree(word, counter) ;
		return;
	}

	NodeData::push_left_subtree(word, counter) ;
	return;
}

void RemoteDataNode::push_right_subtree(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->push_right_subtree(word, counter) ;
		return;
	}

	NodeData::push_right_subtree(word, counter) ;
	return;
}

void RemoteDataNode::pull_left_subtree() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->pull_left_subtree() ;
		return;
	}

	NodeData::pull_left_subtree() ;
	return;
}

void RemoteDataNode::pull_right_subtree() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->pull_right_subtree() ;
		return;
	}

	NodeData::pull_right_subtree() ;
	return;
}

void RemoteDataNode::set_subtree(NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->set_subtree(subtree_parser_.parseProtoTree(tree)) ;
		return;
	}

	NodeData::set_subtree(tree) ;
	return;
}

void RemoteDataNode::set_left_subtree(NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		client_->set_left_subtree(subtree_parser_.parseProtoTree(tree)) ;
		return;
	}

	NodeData::set_left_subtree(tree) ;
	return;
}

void RemoteDataNode::set_right_subtree(NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		std::shared_ptr<sr::PabloSubtree> proto_tree = subtree_parser_.parseProtoTree(tree);
		client_->set_right_subtree(proto_tree) ;
		return;
	}

	NodeData::set_right_subtree(tree) ;
	return;
}

NodeData::NodePtr RemoteDataNode::find_successor() {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		SubtreeMessageParser::ProtoNodePtr proto_node = client_->find_successor();
		return proto_node ? subtree_parser_.parseNodeTree(*proto_node.get()) : nullptr;
	}

	return NodeData::find_successor() ;
}

NodeData::NodePtr RemoteDataNode::clear() {
	LOG(INFO) << __FUNCTION__;

	if (client_) {
		client_->clear();
		return std::dynamic_pointer_cast<NodeData>(shared_from_this());
	}

    if (left_child_) {
        left_child_ = left_child_->clear();
    }

    if (right_child_) {
        right_child_ = right_child_->clear();
    }

    setWord(std::string());
    counter_ = 0;
    height_ = -1;
    return std::dynamic_pointer_cast<NodeData>(shared_from_this());
}

void RemoteDataNode::preorder() {
	LOG(INFO) << __FUNCTION__;
	if (client_) {
		client_->preorder();
	}

	NodeData::preorder() ;
	return;
}
