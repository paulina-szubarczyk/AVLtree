#include <tree/NodeData.h>
#include "glog/logging.h"
#include <algorithm>

NodeData::NodeData()
				: left_child_(nullptr), right_child_(nullptr), parent_(nullptr),
				  height_(-1),  word_(std::string()), counter_(0)
{
	LOG(INFO) << __FUNCTION__;

}

NodeData::NodeData(std::string word)
	:  left_child_(nullptr), right_child_(nullptr), parent_(nullptr),
	   height_(0), word_(word), counter_(1)
{
	LOG(INFO) << __FUNCTION__;
}

NodeData::NodeData(std::string word, int counter, NodeData::NodePtr parent)
	:  left_child_(nullptr), right_child_(nullptr),
	   height_(0), word_(word), counter_(counter)
{
	LOG(INFO) << __FUNCTION__;
	LOG(INFO) << "height: " << height_ << " word: " << word_ <<  " counter: " << counter_ ;
}


int NodeData::getHeight() {
	LOG(INFO) << __FUNCTION__ << " " << height_;

	return height_;
}

void NodeData::setLeft(NodeData::NodePtr left) {

	left_child_ = left;
}

void NodeData::setRight(NodeData::NodePtr right) {
	LOG(INFO) << __FUNCTION__;

	right_child_ = right;
}

void NodeData::setParent(NodeData::NodePtr parent) {
	LOG(INFO) << __FUNCTION__;

	parent_ = parent;
}

NodeData::NodePtr NodeData::getLeft() {
	LOG(INFO) << __FUNCTION__ << " " << ((left_child_ != nullptr) ? left_child_->getWord() : "empty");

	return left_child_;
}

NodeData::NodePtr NodeData::getRight() {
	LOG(INFO) << __FUNCTION__<<  " " << ((right_child_ != nullptr) ? right_child_->getWord() : "empty");

	return right_child_;
}

NodeData::NodePtr NodeData::getParent() {
	LOG(INFO) << __FUNCTION__;

	return parent_;
}

std::string NodeData::getWord() {
	LOG(INFO) << __FUNCTION__ << " " << word_;

	return word_;
}

void NodeData::setWord(std::string word) {
	LOG(INFO) << __FUNCTION__;

	word_ = word;
	++counter_;
	height_ = 0;

	LOG(INFO) << " height " << height_ << " word " <<  word << " counter " << counter_;
}

void NodeData::setData(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__ << " " << word << " " << counter;

	word_ = word;
	counter_ = counter;
	height_ = 0;
}


bool NodeData::hasWord() {
	LOG(INFO) << __FUNCTION__;

	return (getHeight() > -1 && getCounter() && getWord() != std::string());
}

bool NodeData::equalWord(const std::string word) {
	LOG(INFO) << __FUNCTION__ ;

	return word.compare(getWord()) == 0;
}

bool NodeData::greaterWord(const std::string word) {
	LOG(INFO) << __FUNCTION__;

	return word.compare(getWord()) > 0;
}

int NodeData::getCounter() {
	LOG(INFO) << __FUNCTION__ << " " <<   counter_ ;

	return counter_;
}

void NodeData::increaseCounter() {
	LOG(INFO) << __FUNCTION__;

	++counter_;
}

void NodeData::decreaseCounter() {
	LOG(INFO) << __FUNCTION__;

	--counter_;
}

NodeData::NodePtr NodeData::create(std::string word, NodeData::NodePtr parent) {
	LOG(INFO) << __FUNCTION__;

	return std::make_shared<NodeData>(word, 1, parent);
}

NodeData::NodePtr NodeData::create( NodeData::NodePtr node) {
	LOG(INFO) << __FUNCTION__;

	return std::make_shared<NodeData>(node->getWord(), node->getCounter(), nullptr);
}

NodeData::NodePtr NodeData::create() {
	LOG(INFO) << __FUNCTION__;

	return std::make_shared<NodeData>();
}

NodeData::NodePtr NodeData::clear() {
	LOG(INFO) << __FUNCTION__;

	return nullptr;
}

void NodeData::insert(std::string word) {
	LOG(INFO) << __FUNCTION__ << " " << word;

	if (!hasWord()) {
		setWord(word);
		return;
	}

	if (equalWord(word)) {
		increaseCounter();
		return;
	}

	if (greaterWord(word)) {
		if(getRight())
			getRight()->insert(word);
		else
			setRight(NodeData::create(word));
	} else {
		if(getLeft())
			getLeft()->insert(word);
		else
			setLeft(NodeData::create(word));
	}

	balance(updateHeight(), word);
	return;
}

void NodeData::remove_local_word(std::string word) {

	LOG(INFO) << __FUNCTION__;


	if (getLeft() && getRight() && getLeft()->hasWord() && getRight()->hasWord())
	{
		NodeData::NodePtr lesser_successor = right_child_->find_successor();

		std::string l_word = lesser_successor->getWord();

		int l_counter = lesser_successor->getCounter();

		setData(l_word, l_counter);

	} else if (getLeft() && getLeft()->hasWord())
	{
		pull_left_subtree();

	} else  if (getRight() && getRight()->hasWord())
	{
		pull_right_subtree();
	} else {
		clearData();
	}
}

void NodeData::remove_remote_word(std::string word) {
	LOG(INFO) << __FUNCTION__;

	if (!greaterWord(word)) {

		if (!left_child_) {
			LOG(WARNING) << "Word not found child";
			return;
		}

		left_child_->remove(word);

		if (left_child_ || !left_child_->hasWord()) {

			left_child_ = left_child_->clear();
		}

	} else {

		if (!right_child_) {
			LOG(WARNING) << "Word not found child";
			return;
		}

		right_child_->remove(word);

		if (right_child_ || !right_child_->hasWord()) {

			right_child_ = right_child_->clear();
		}

	}
}

void NodeData::remove(std::string word) {
	LOG(INFO) << __FUNCTION__;

	if (equalWord(word)) {

		decreaseCounter();

		if (counter_ <= 0)
		{
			if ((!getLeft() && !getRight()) || (!getLeft()->hasWord() && !getRight()->hasWord()))
			{
				LOG(INFO) << __FUNCTION__ << word;
				clearData();
				return;
			}

			remove_local_word(word);
		}
	} else {

		remove_remote_word(word);

	}
}

int NodeData::updateHeight() {
	LOG(INFO) << __FUNCTION__ << " " << word_;

	int left_height = left_child_ ? left_child_->getHeight() : -1;
	int right_height = right_child_ ? right_child_->getHeight() : -1;
	height_ = std::max(left_height,right_height) + 1;

	LOG(INFO) << __FUNCTION__ << " balance: right: " << right_height ;
	LOG(INFO) << __FUNCTION__ << " balance: left: " << left_height;
	LOG(INFO) << __FUNCTION__ << " balance: node: " << height_;
	LOG(INFO) << __FUNCTION__ << " balance: " << right_height - left_height;
	return right_height - left_height;
}

bool NodeData::smaller(NodeData::NodePtr node, std::string word) {
	LOG(INFO) << __FUNCTION__;

	return node && node->hasWord() && word.compare(node->getWord()) < 0;
}

void NodeData::balance(int balance_value, std::string new_word) {
	LOG(INFO) << __FUNCTION__ << balance_value;

	//TODO check it
	if (balance_value == 2) {
		if (smaller(right_child_, new_word))
			right_child_->rotate_right();
		rotate_left();

	} else if (balance_value == -2) {
		if (!smaller(left_child_, new_word))
			left_child_->rotate_left();
		rotate_right();
	}
}

void NodeData::balance_remove(int balance_value, std::string new_word) {
	LOG(INFO) << __FUNCTION__;

	//TODO check it
	if (balance_value == 2) {
		if (!smaller(right_child_, new_word))
			right_child_->rotate_right();
		rotate_left();

	} else if (balance_value == -2) {
		if (smaller(left_child_, new_word))
			left_child_->rotate_left();
		rotate_right();
	}
}

void NodeData::rotate_right() {
	LOG(INFO) << __FUNCTION__;

	if (!left_child_)
		LOG(WARNING) << "There isn't left child";

    std::string word = left_child_->getWord();

    int counter = left_child_->getCounter();

    if (!right_child_) {
        right_child_= NodeData::create();
        right_child_->setData(getWord(),getCounter());
    } else {
        right_child_->push_right_subtree(getWord(), getCounter());
    }
    if (left_child_) {
        NodeData::NodePtr left_tree = left_child_->get_right_subtree();
        if (left_tree) {
            right_child_->set_left_subtree(left_tree);
        }
        if (!left_child_->getLeft() && !left_child_->getRight())
        {
        	left_child_ = left_child_->clear();
        }
        else {
        	left_child_->pull_left_subtree();
        }
    }
    setData(word, counter);
    updateHeight();
}

void NodeData::rotate_left() {
	LOG(INFO) << __FUNCTION__;

	if (!right_child_)
		LOG(WARNING) << "There isn't right child";

    std::string word = right_child_->getWord();

    int counter = right_child_->getCounter();

    if (!left_child_) {
        left_child_ = NodeData::create();
		left_child_->setData(getWord(),getCounter());
    } else {
        left_child_->push_left_subtree(getWord(), getCounter());
    }
    if (right_child_) {
    	NodeData::NodePtr right_tree = right_child_->get_left_subtree();
        if (right_tree) {
            left_child_->set_right_subtree(right_tree);
        }
        if (!right_child_->getLeft() && !right_child_->getRight())
        {
        	right_child_ = right_child_->clear();
        }
        else {
        	right_child_->pull_right_subtree();
        }

    }
    setData(word, counter);
    updateHeight();
}

void NodeData::push_left_subtree(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;

	if (!hasWord()) {
		setData(word, counter);
		return;
	}
	if (!left_child_) {
		left_child_ = NodeData::create();
		left_child_->setData(getWord(),getCounter());
	} else {
		left_child_->push_left_subtree(getWord(), getCounter());
	}
	if (right_child_) {
		NodeData::NodePtr tree = right_child_->get_subtree();
		right_child_ = right_child_->clear();
		if (tree) {
			left_child_->set_subtree(tree);
		}
	}
	setData(word, counter);
	height_++;

}

void NodeData::push_right_subtree(std::string word, int counter) {
	LOG(INFO) << __FUNCTION__;

	if (!hasWord()) {
		setData(word, counter);
		return;
	}
	if (!right_child_) {
		right_child_ =  NodeData::create();
		right_child_->setData(getWord(),getCounter());
	} else {
		right_child_->push_right_subtree(getWord(), getCounter());
	}
	if (left_child_) {
		NodeData::NodePtr tree = left_child_->get_subtree();
		left_child_ = left_child_->clear();
		if (tree) {
			right_child_->set_subtree(tree);
		}
	}
	setData(word, counter);
	height_++;
}

void NodeData::pull_left_subtree() {
	LOG(INFO) << __FUNCTION__;

	if (right_child_) {
		right_child_ = right_child_->clear();
	}
	if (left_child_) {

		std::string word = left_child_->getWord();
		int counter = left_child_->getCounter();


	 	NodeData::NodePtr right_tree = left_child_->get_right_subtree();
	 	NodeData::NodePtr left_tree = left_child_->get_left_subtree();
	 	left_child_ = left_child_->clear();
	 	if (right_tree) {
	 		set_right_subtree(right_tree);
	 	}
	 	if (left_tree) {
	 		set_right_subtree(left_tree);
	 	}

	 	setData(word, counter);
	 	updateHeight();
	} else {
		clearData();
	}
}

void NodeData::pull_right_subtree() {
	LOG(INFO) << __FUNCTION__;

	if (left_child_) {
		left_child_ = left_child_->clear();
	}
	if (right_child_) {

		std::string word = right_child_->getWord();
		int counter = right_child_->getCounter();

	 	NodeData::NodePtr right_tree = right_child_->get_right_subtree();
	 	NodeData::NodePtr left_tree = right_child_->get_left_subtree();
	 	right_child_ = right_child_->clear();
	 	if (right_tree) {
	 		set_right_subtree(right_tree);
	 	}
	 	if (left_tree) {
	 		set_right_subtree(left_tree);
	 	}

	 	setData(word, counter);
	 	updateHeight();
	} else {
		clearData();
	}
}

void NodeData::set_subtree(NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;

	if(tree) {
		setData(tree->getWord(),tree->getCounter());
		setLeft(tree->getLeft());
		setRight(tree->getRight());
	}
}

void NodeData::set_left_subtree(NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;

	if(tree) {
        if (left_child_) {
            left_child_ = left_child_->clear();
        }
        if (!left_child_) {
            left_child_ = tree;
        } else {
            left_child_->setData(tree->getWord(), tree->getCounter());
        }
        left_child_->set_subtree(tree);
        updateHeight();
	}
}

void NodeData::set_right_subtree(NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;

	if(tree) {
        if (right_child_) {
            right_child_ = right_child_->clear();
        }
        if (!right_child_) {
            right_child_ = tree;
        } else {
            right_child_->setData(tree->getWord(), tree->getCounter());
        }
        right_child_->set_subtree(tree);
        updateHeight();
	}
}

NodeData::NodePtr NodeData::find_successor() {
	LOG(INFO) << __FUNCTION__;

    if (!getLeft() && !getLeft()->hasWord()) {
    	NodeData::NodePtr value = NodeData::create(getWord());
    	value->setData(getWord(), getCounter());
    	pull_right_subtree();
    	balance(updateHeight(),getWord());
        return value;
    }
	NodeData::NodePtr lesser_successor = left_child_->find_successor();
	return lesser_successor;
}

NodeData::NodePtr NodeData::get_subtree() {
	LOG(INFO) << __FUNCTION__;

	NodePtr node = NodeData::create();
	node->setData(getWord(),getCounter());
	node->setLeft(getLeft());
	node->setRight(getRight());
	node->updateHeight();
	return node;
}

NodeData::NodePtr NodeData::get_right_subtree() {
	LOG(INFO) << __FUNCTION__;

	return right_child_ ? right_child_->get_subtree() : nullptr;
}

NodeData::NodePtr NodeData::get_left_subtree() {
	LOG(INFO) << __FUNCTION__;

	return left_child_ ? left_child_->get_subtree() : nullptr;
}

void NodeData::preorder() {
	LOG(INFO) << __FUNCTION__ << "word: " << getWord() << " height: " <<  getHeight() << " counter: " << getCounter();

	if (left_child_) {
		LOG(INFO) << " GO LEFT ";

		left_child_->preorder();

	}

	if (right_child_) {
		LOG(INFO) << " GO RIGHT ";

		right_child_->preorder();

	}

	LOG(INFO) << " GO UP ";

}
