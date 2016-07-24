#include <network/SubtreeMessageParser.h>

SubtreeMessageParser::SubtreeMessageParser() {
	// TODO Auto-generated constructor stub
	LOG(INFO) << __FUNCTION__;

}

SubtreeMessageParser::~SubtreeMessageParser() {
	// TODO Auto-generated destructor stub
	LOG(INFO) << __FUNCTION__;

}


NodeData::NodePtr SubtreeMessageParser::parseTree(
		const sr::PabloSubtree& tree) {
	LOG(INFO) << __FUNCTION__;

	NodeData::NodePtr root = tree.has_node() ? parseNodeTree(tree.node()) : nullptr;

	if (root && tree.has_left()) {
	   root->setLeft(parseTree(tree.left()));
	}

	if (root && tree.has_right()) {
	   root->setRight(parseTree(tree.right()));
	}
	return root;
}

NodeData::NodePtr SubtreeMessageParser::parseNodeTree(
		const sr::DataNodeMessage& tree) {
	LOG(INFO) << __FUNCTION__;

	if (tree.counter() == 0) {
		return nullptr;
	}

	NodeData::NodePtr node = NodeData::create();

	node->setData(tree.word(), tree.counter());


	return node;
}

SubtreeMessageParser::ProtoTreePtr SubtreeMessageParser::parseProtoTree(
		NodeData::NodePtr tree) {
	LOG(INFO) << __FUNCTION__;

	ProtoTreePtr proto_tree = std::make_shared<sr::PabloSubtree>();

	if (!tree) {
		return proto_tree;
	}

	sr::DataNodeMessage* node = proto_tree->mutable_node();

	node->set_word(tree->getWord());
	node->set_counter(tree->getCounter());

	if (tree->getLeft()) {
	   sr::PabloSubtree* left_child = proto_tree->mutable_left();
	   *left_child = *parseProtoTree(tree->getLeft());
	}

	if (tree->getRight()) {
		sr::PabloSubtree* right_child = proto_tree->mutable_right();
		*right_child = *parseProtoTree(tree->getRight());
	}
	return proto_tree;
}

SubtreeMessageParser::ProtoNodePtr SubtreeMessageParser::parseProtoNode(NodeData::NodePtr node) {
	LOG(INFO) << __FUNCTION__;

	ProtoNodePtr proto_node = createNode();

	proto_node->set_word(node->getWord());
	proto_node->set_counter(node->getCounter());

	return proto_node;
}
