#include <src/FileManager.h>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <istream>
#include <fstream>
#include <iostream>
#include <string>

#include "glog/logging.h"

FileManager::FileManager(std::shared_ptr<AVLTree> avl_tree, std::shared_ptr<Sender> sender, bool root)
: avl_tree_(avl_tree), sender_(sender), root(root), test_(true){
	LOG(INFO) << __FUNCTION__;

}

FileManager::~FileManager() {
	LOG(INFO) << __FUNCTION__;
}

void FileManager::start() {
	LOG(INFO) << __FUNCTION__;

	if(root) {
		avl_tree_->run() ;
	}

	if(test_) {
		test();
	} else {
		readFile();
	}

}

void FileManager::test() {

	std::vector<std::string> words2 = {"MMM", "TTT", "DDD", "CCC", "HHH",
			"WWW", "VVV", "EEE", "FFF", "BBB", "FFF", "NNN", "RRR", "BBB",
			"QQQ", "EEE", "III", "XXX", "AAA", "PPP", "ZZZ", "TTT"};

	for (auto word : words2) {
		root ? avl_tree_->insert(word) : sender_->write(Sender::operation::ADD, word);
	}

//	for (auto word : words2) {
//		root ? avl_tree_->remove(word) : sender_->write(Sender::operation::REMOVE, word);
//	}

}

void FileManager::readFile() {
	LOG(INFO) << __FUNCTION__;


	std::ifstream file(file_);

	std::string line;

	std::vector<std::string> words;

	while (std::getline(file,line) )
	{

		boost::split(words, line, boost::is_any_of(" "));

		for(auto word : words) {
			root ? avl_tree_->insert(word) : sender_->write(Sender::operation::ADD, word);
		}
		words.clear();
	}

	file.close();

	std::ifstream file2(file_);

	while(std::getline(file2, line) ) {

		boost::split(words, line, boost::is_any_of(" "));

		for(auto word : words) {
//			root ? avl_tree_->remove(word) : sender_->write(Sender::operation::REMOVE, word);
		}
		words.clear();
	}
	file2.close();
}
