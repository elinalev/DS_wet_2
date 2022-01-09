//
// Created by lior1 on 1/8/2022.
//
#include "group.h"
#ifndef MTM_2_PARTB_UNION_FIND_H
#define MTM_2_PARTB_UNION_FIND_H

#define NO_PARENT -1
class UnionFind{
    //k = number of groups+1
    int k;
    //an array of size as number of groups+1
    int* size;
    //an array of size as number of groups+1
    int* parent;
    //an array of groups of size as number of groups+1
    Group* groups;

    int getRoot(int x);

public:

    ///todo: function for debug, remove it
    void print_union(){
        printf("\n%d\n", k);
        for (int i = 0; i < k; i++)
            printf ("%d  ",size[i]);
    }
    explicit UnionFind(int k);
    int find(int x);
    StatusType unite(int root1, int root2);

    ~UnionFind(){
        delete[] size;
        delete[] parent;
        delete[] groups;
    }
    /// get the group related to a root of the tree
    /// \param root - the key we want the struct of
    /// \return - required group in case of success, group with no trees inside in failure
    Group& get_group(int root);
};

UnionFind::UnionFind(int k): k(k+1){
    size = new int[k+1];
    parent = new int[k+1];
    for(int i = 0; i < k+1; i++) {
        size[i] = 1;
        parent[i] = -1;
    }
    groups = new Group[k+1];
}

int UnionFind::getRoot(int x){
    while(parent[x] != -1)
        x = parent[x];
    return x;
}
int UnionFind::find(int x){
    if(x<0 || x > k)
        return -1;
    int root = getRoot(x), temp;
    while(parent[x] != -1){
        temp = parent[x];
        parent[x] = root;
        x = temp;
    }
    return x;
}

StatusType UnionFind::unite(int root1, int root2){
    if(root1 < 0 || root2 < 0 || root1 > k || root2 > k || root1 == root2)
        return INVALID_INPUT;
    int bigger, smaller;
    if(size[root1] > size[root2])
        bigger = root1, smaller = root2;
    else
        bigger = root2, smaller = root1;
    parent[smaller] = bigger;
    size[bigger] += size[smaller];
    size[smaller] = -1;
    StatusType status = groups[bigger].merge(groups[smaller]);
    groups[smaller].remove_group();
    return status;
}


Group& UnionFind::get_group(int root){
    return groups[root];
}

#endif //MTM_2_PARTB_UNION_FIND_H
