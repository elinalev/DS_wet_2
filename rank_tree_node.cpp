//
// Created by lior1 on 1/8/2022.
//


#include "rank_tree_node.h"


bool RankTreeNode::operator==(const RankTreeNode& other) const{
    return other.key == key;
}
bool RankTreeNode::operator>(const RankTreeNode& other) const{
    return key > other.key;
}
bool RankTreeNode::operator<(const RankTreeNode& other) const{
    return key < other.key;
}

int RankTreeNode::get_balance_factor(){
    int left_h = left == nullptr? -1 : left->height;
    int right_h = right == nullptr? -1 : right->height;
    return left_h - right_h;
}
int RankTreeNode::cal_height_non_recursive(){
    int left_h = left == nullptr? -1 : left->height;
    int right_h = right == nullptr? -1 : right->height;
    return height = max(left_h, right_h) +1;
}
void RankTreeNode::get_size_of_subtree(){
    int left_size = left ?left->size_of_subtree : 0;
    int right_size = right?right->size_of_subtree : 0;
    size_of_subtree = (key? left_size + right_size + value : left_size + right_size);
}
void RankTreeNode::get_sum_of_subtree(){
    int left_sum = left? left->sum_of_subtree : 0;
    int right_sum = right? right->sum_of_subtree : 0;
    sum_of_subtree = left_sum + right_sum + ((key) * (value));
}
void RankTreeNode::update_details(){
    cal_height_non_recursive();
    get_size_of_subtree();
    get_sum_of_subtree();
}


int RankTreeNode::num_of_smaller_then_this(){
    if(!left)
        return 0;
    return left->size_of_subtree;
}

int RankTreeNode::sum_of_smaller_then_this(){
    if(!left)
        return 0;
    return left->sum_of_subtree;
}

