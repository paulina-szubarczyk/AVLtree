#ifndef APP_TREE_AVLTREE_H_
#define APP_TREE_AVLTREE_H_
#include <utility>
#include <tree/NodeData.h>
#include <boost/lockfree/queue.hpp>
#include <tree/RemoteDataNode.h>
#include <boost/function.hpp>
#include <boost/lockfree/queue.hpp>

class RemoteDataNode;

class AVLTree {
public:
	AVLTree();

	virtual ~AVLTree();

	void setRoot(NodeData::NodePtr root);

	NodeData::NodePtr getRoot();

	void insert( std::string word);

	void remove( std::string word);

	void preorder();

	void run();

	enum operation {
		ADD,
		REMOVE
	};

private:

	NodeData::NodePtr root_;

	boost::lockfree::queue<operation> queue_;
	std::vector<std::string> words_;

	void invoke();
};

#endif /* APP_TREE_AVLTREE_H_ */
