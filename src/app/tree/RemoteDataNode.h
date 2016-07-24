#ifndef APP_TREE_REMOTEDATANODE_H_
#define APP_TREE_REMOTEDATANODE_H_

#include "tree/NodeData.h"
#include "network/RemoteAVLTreeClient.h"
#include "network/SubtreeMessageParser.h"

class RemoteDataNode: public NodeData{
public:

	typedef std::shared_ptr<RemoteDataNode> remote_ptr;

	RemoteDataNode(RemoteAVLTreeClient::client_ptr client) ;
	 ~RemoteDataNode();

	static NodePtr create(RemoteAVLTreeClient::client_ptr client);

	void setData(std::string word, int counter);

	std::string getWord();
	int getCounter();
	int getHeight();

	void insert(std::string word);
	void remove(std::string word);

	void rotate_left();
	void rotate_right();

	void push_left_subtree(std::string word, int counter);
	void push_right_subtree(std::string word, int counter);

	void pull_left_subtree();
	void pull_right_subtree();

	void set_subtree(NodeData::NodePtr tree);
	void set_left_subtree(NodeData::NodePtr tree);
	void set_right_subtree(NodeData::NodePtr tree);

	NodeData::NodePtr get_subtree();
	NodeData::NodePtr get_right_subtree();
	NodeData::NodePtr get_left_subtree();

	NodeData::NodePtr find_successor();

	NodeData::NodePtr clear() ;

	void preorder();
private:
	RemoteAVLTreeClient::client_ptr client_;

	SubtreeMessageParser subtree_parser_;

};

#endif /* APP_TREE_REMOTEDATANODE_H_ */
