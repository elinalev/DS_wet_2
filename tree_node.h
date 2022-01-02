#ifndef DS_WET_1_TREE_NODE_H
#define DS_WET_1_TREE_NODE_H

#include <memory>
#include "utils.h"

template<class T, class S>
class TreeNode{
public:
    T key;
    S value;
    std::shared_ptr<TreeNode> right, left;
    int height;
    int size_of_subtree;
    int sum_of_subtree;

    TreeNode();

    bool operator==(const TreeNode& other){
        return other.key == key;
    }
    bool operator>(const TreeNode& other){
        return key > other.key;
    }
    bool operator<(const TreeNode& other){
        return key < other.key;
    }

    int get_balance_factor(){
        int left_h = left == nullptr? -1 : left->height;
        int right_h = right == nullptr? -1 : right->height;
        return left_h - right_h;
    }
    int cal_height_non_recursive(){
        int left_h = left == nullptr? -1 : left->height;
        int right_h = right == nullptr? -1 : right->height;
        return max(left_h, right_h) +1;
    }
};

template<class T, class S>
TreeNode<T,S>::TreeNode() :right(nullptr), left(nullptr), height(0){}

#endif //DS_WET_1_TREE_NODE_H