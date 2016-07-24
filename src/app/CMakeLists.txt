cmake_minimum_required(VERSION 2.8)

project( App )

# Include directories for easier handling of includes
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

# Boost
find_package( Boost 1.54 COMPONENTS system thread filesystem REQUIRED)

include_directories( ${Boost_INCLUDE_DIR} )

link_directories( ${Boost_LIBRARY_DIRS} )

add_subdirectory(proto)

add_subdirectory(src)

add_subdirectory(network)

add_subdirectory(tree)

