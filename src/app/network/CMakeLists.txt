cmake_minimum_required(VERSION 2.8)
project(Network)
include_directories( 
                 ${Message_BINARY_DIR}
                 ${Tree_BINARY_DIR})

add_library(networklib 
                        MessageParser.cpp
                        MessageHandler.cpp
                        MessageObserver.cpp
                        Endpoint.cpp
                        AVLMessageHandler.cpp
                        AVLResponseHandler.cpp
                        DummyMessageHandler.cpp
                        RootMessageHandler.cpp
                        InitTreeHandler.cpp
                        HelloMessageHandler.cpp
                        Connection.cpp
                        ThreadPool.cpp
                        RemoteAVLTreeClient.cpp
                        RemoteTreeManager.cpp
                        RootDutiesManager.cpp
                        SenderMessageHandler.cpp
                        Server.cpp          
                        SubtreeMessageParser.cpp
                        Sender.cpp
                              )
                    
target_link_libraries(networklib ${GLOG_LIBRARIES} ${Boost_LIBRARIES} treelib)

install(TARGETS networklib
               RUNTIME DESTINATION bin
               LIBRARY DESTINATION lib
               ARCHIVE DESTINATION lib/static
)

