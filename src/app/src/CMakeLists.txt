cmake_minimum_required(VERSION 2.8)
project( src )

include_directories(${Message_BINARY_DIR} ${Network_BINARY_DIR})

add_executable(DistributedAVLTree
                     FileManager.cpp
                     main.cpp)

target_link_libraries(DistributedAVLTree ${Boost_LIBRARIES} message networklib)

install(TARGETS DistributedAVLTree
               RUNTIME DESTINATION bin
               LIBRARY DESTINATION lib
               ARCHIVE DESTINATION lib/static
)

