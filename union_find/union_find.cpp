#include "union_find.h"

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
    if(root1 <= 0 || root2 <= 0 || root1 > k || root2 > k)
        return INVALID_INPUT;
    if(root1 == root2)
        return SUCCESS;
    try {
        int bigger, smaller;
        if (size[root1] > size[root2])
            bigger = root1, smaller = root2;
        else
            bigger = root2, smaller = root1;
        parent[smaller] = bigger;
        size[bigger] += size[smaller];
        size[smaller] = -1;
        StatusType status = groups[bigger].merge(groups[smaller]);
        groups[smaller].remove_group();
        return status;
    }catch (const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
}


Group& UnionFind::get_group(int root){
    root = getRoot(root);
    return groups[root];
}
