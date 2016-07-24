cmake_minimum_required(VERSION 2.8)
project(Tree)
include_directories( 
                 ${Message_BINARY_DIR}
                 ${Network_BINARY_DIR})
add_library(treelib
                    RemoteDataNode.cpp
                    NodeData.cpp
                    AVLTree.cpp)
                    
target_link_libraries(treelib ${GLOG_LIBRARIES} ${Boost_LIBRARIES} networklib)

install(TARGETS treelib
               RUNTIME DESTINATION bin
               LIBRARY DESTINATION lib
               ARCHIVE DESTINATION lib/static
)

