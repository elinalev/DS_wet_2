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

    bool operator==(const RankTreeNode& other) const;
    bool operator>(const RankTreeNode& other) const;
    bool operator<(const RankTreeNode& other) const;

    int get_balance_factor();
    int cal_height_non_recursive();
    void get_size_of_subtree();
    void get_sum_of_subtree();
    std::shared_ptr<RankTreeNode> update_details();
    int num_of_smaller_then_this();
    int sum_of_smaller_then_this();

};


#endif //DS_WET_1_TREE_NODE_H