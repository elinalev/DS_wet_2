#ifndef DS_WET_1_TREE_NODE_H
#define DS_WET_1_TREE_NODE_H

#include <memory>
#include "utils.h"

class TreeNode{
public:
    int key;
    int value;
    std::shared_ptr<TreeNode> right, left;
    int height;
    int size_of_subtree;
    int sum_of_subtree;

    explicit TreeNode(int key);

    bool operator==(const TreeNode& other) const{
        return other.key == key;
    }
    bool operator>(const TreeNode& other) const{
        return key > other.key;
    }
    bool operator<(const TreeNode& other) const{
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

TreeNode::TreeNode(int key) :right(nullptr), left(nullptr), height(0),key(key), value(0), size_of_subtree(0), sum_of_subtree(0){}

#endif //DS_WET_1_TREE_NODE_H