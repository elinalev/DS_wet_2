#ifndef DS_WET_1_TREE_NODE_H
#define DS_WET_1_TREE_NODE_H

#include <memory>
#include "utils.h"

class RankTreeNode{
public:
    int key;
    int value;
    std::shared_ptr<RankTreeNode> right, left;
    int height;
    int size_of_subtree;
    int sum_of_subtree;

    explicit RankTreeNode(int key);

    bool operator==(const RankTreeNode& other) const{
        return other.key == key;
    }
    bool operator>(const RankTreeNode& other) const{
        return key > other.key;
    }
    bool operator<(const RankTreeNode& other) const{
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
        return height = max(left_h, right_h) +1;
    }
    void get_size_of_subtree(){
        int left_size = left ?left->size_of_subtree : 0;
        int right_size = right?right->size_of_subtree : 0;
        size_of_subtree = (key? left_size + right_size + value : left_size + right_size);
    }
    void get_sum_of_subtree(){
        int left_sum = left? left->sum_of_subtree : 0;
        int right_sum = right? right->sum_of_subtree : 0;
        sum_of_subtree = left_sum + right_sum + ((key) * (value));
    }
    void update_details(){
        cal_height_non_recursive();
        get_size_of_subtree();
        get_sum_of_subtree();
    }
};

RankTreeNode::RankTreeNode(int key) : key(key), value(0), right(nullptr) , left(nullptr), height(0), size_of_subtree(0), sum_of_subtree(0){}

#endif //DS_WET_1_TREE_NODE_H