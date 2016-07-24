#ifndef APP_SRC_FILEMANAGER_H_
#define APP_SRC_FILEMANAGER_H_
#include <tree/AVLTree.h>
#include <network/Sender.h>

class FileManager {
public:
	FileManager(std::shared_ptr<AVLTree> avl_tree, std::shared_ptr<Sender> sender, bool root);
	virtual ~FileManager();

	void set_file(std::string file) {
		file_ = file;
		test_ = false;
	}
	void start();

private:
	std::shared_ptr<AVLTree> avl_tree_;
	std::shared_ptr<Sender> sender_;
	bool root, test_;
	std::string file_;

	void readFile();
	void test();
};

#endif /* APP_SRC_FILEMANAGER_H_ */
