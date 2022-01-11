cmake_minimum_required(VERSION 2.8.4)
project(mtm_2_partb)

set(CMAKE_CXX_STANDARD 11)

set(MTM_FLAGS_DEBUG "-std=c++11 -Wall -Werror --coverage")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_CXX_FLAGS ${MTM_FLAGS_DEBUG})

#set(CPP_FILES union_find/union_test.cpp rank_tree.cpp rank_tree_node.cpp utils.cpp)
set(CPP_FILES union_find/group.cpp union_find/union_find.cpp
        rank_tree.cpp rank_tree_node.cpp utils.cpp library2.cpp main2.cpp player.cpp
        PlayersManager.cpp )
add_executable(mtm_2_partb ${CPP_FILES})