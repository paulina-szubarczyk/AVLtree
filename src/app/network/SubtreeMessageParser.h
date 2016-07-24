#ifndef APP_NETWORK_SUBTREEMESSAGEPARSER_H_
#define APP_NETWORK_SUBTREEMESSAGEPARSER_H_
#include "protocol.pb.h"
#include "tree/NodeData.h"
#include "glog/logging.h"

class SubtreeMessageParser {
public:
	SubtreeMessageParser();
	virtual ~SubtreeMessageParser();

	typedef std::shared_ptr<sr::PabloSubtree> ProtoTreePtr;
	typedef std::shared_ptr<sr::DataNodeMessage> ProtoNodePtr;


	NodeData::NodePtr parseTree(const sr::PabloSubtree& tree);
	NodeData::NodePtr parseNodeTree(const sr::DataNodeMessage& tree);
	ProtoTreePtr parseProtoTree(NodeData::NodePtr tree);
	ProtoNodePtr parseProtoNode(NodeData::NodePtr tree);


	ProtoTreePtr createTree() {
		LOG(INFO) << __FUNCTION__;

		return std::make_shared<sr::PabloSubtree>();
	}

	ProtoNodePtr createNode() {
		LOG(INFO) << __FUNCTION__;

		return std::make_shared<sr::DataNodeMessage>();
	}
};

#endif /* APP_NETWORK_SUBTREEMESSAGEPARSER_H_ */
