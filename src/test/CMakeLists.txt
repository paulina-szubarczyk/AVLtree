project(Test)

# Compile gtest
add_subdirectory(gtest)

include_directories( 
    ${gtest_INCLUDE_DIR} 
    ${tree_SOURCE_DIR}
)

set(TEST_SOURCES 
    test.cpp
)


add_executable(test ${TEST_SOURCES})

target_link_libraries(test gtest_main treelib)

add_custom_command(TARGET test POST_BUILD COMMAND test)