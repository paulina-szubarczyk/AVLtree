#ifndef APP_TREE_NODEDATA_H_
#define APP_TREE_NODEDATA_H_
#include <memory>

class NodeData : public std::enable_shared_from_this<NodeData> {

public:

	enum Child {
		LEFT,
		RIGHT
	};

	typedef std::shared_ptr<NodeData> NodePtr;

	NodeData();
	NodeData(std::string word);
	NodeData(std::string word, int counter, NodeData::NodePtr parent);

	virtual ~NodeData() {}

	static NodeData::NodePtr create(std::string word, NodeData::NodePtr parent = nullptr);
	static NodeData::NodePtr create(NodeData::NodePtr node);
	static NodeData::NodePtr create();


	virtual void setWord(std::string word);
	virtual void setData(std::string word, int counter);

	virtual std::string getWord();
	virtual bool hasWord();
	virtual bool equalWord(const std::string word);
	virtual bool greaterWord(const std::string word);

	virtual void increaseCounter();
	virtual void decreaseCounter();

	virtual int getCounter();
	virtual int getHeight();
	virtual int updateHeight();

	virtual void setLeft(NodePtr);
	virtual void setRight(NodePtr);
	virtual void setParent(NodePtr parent);

	virtual NodePtr getLeft();
	virtual NodePtr getRight();
	virtual NodePtr getParent();

	virtual void insert(std::string word);
	virtual void balance(int balance_val, std::string new_word);
	virtual void balance_remove(int balance_val, std::string new_word);

	virtual void rotate_left();
	virtual void rotate_right();

	virtual void push_left_subtree(std::string word, int counter);
	virtual void push_right_subtree(std::string word, int counter);

	virtual void pull_left_subtree();
	virtual void pull_right_subtree();

	virtual void set_subtree(NodeData::NodePtr tree);
	virtual void set_left_subtree(NodeData::NodePtr tree);
	virtual void set_right_subtree(NodeData::NodePtr tree);

	virtual NodeData::NodePtr get_subtree();
	virtual NodeData::NodePtr get_right_subtree();
	virtual NodeData::NodePtr get_left_subtree();

	virtual NodeData::NodePtr find_successor();

	virtual NodeData::NodePtr clear();

	virtual void preorder();
	virtual void remove(std::string word);

protected:
	NodePtr left_child_;
	NodePtr right_child_;
	NodePtr parent_;
	int height_;
	std::string word_;
	int counter_;

	void remove_local_word(std::string word);
	void remove_remote_word(std::string word);
	bool smaller(NodeData::NodePtr node, std::string word);

	void clearData() {
		word_ = std::string();
		counter_ = 0;
		height_ = -1;
		left_child_ = left_child_ ? left_child_->clear() : nullptr;
		right_child_ = right_child_ ? right_child_->clear() : nullptr;
	}
};

#endif /* APP_TREE_NODEDATA_H_ */
