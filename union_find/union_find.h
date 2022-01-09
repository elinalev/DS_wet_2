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

#endif //MTM_2_PARTB_UNION_FIND_H
