//
// Created by lior1 on 1/8/2022.
//
#include "group.h"
#ifndef MTM_2_PARTB_UNION_NODE_H
#define MTM_2_PARTB_UNION_NODE_H
struct UFNode{
    UFNode* father;
    std::shared_ptr<Group> group;
    UFNode():father(nullptr), group(){}
    UFNode* getRoot();
    UFNode* find();
};

UFNode* UFNode::getRoot(){
    UFNode* root = this;
    while(root)
        root = root->father;
    return root;
}

UFNode* UFNode::find(){
    UFNode* root = getRoot(), *curr = this, *temp;
    while(curr->father) {
        temp = curr->father;
        curr->father = root;
        curr = temp;
    }
    return curr;
}

#endif //MTM_2_PARTB_UNION_NODE_H
