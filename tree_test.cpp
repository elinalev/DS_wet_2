#include "rank_tree.h"
#include <assert.h>
RankTree test_create_int_int(int n){
    RankTree tree = RankTree(n);
    int* arr = new int[n];
    for(int i = 0; i < n; i ++){
        arr[i] = i;
    }
    tree.put_inorder(arr, arr, n);
    return tree;
}

int calculate_height_aux(std::shared_ptr<RankTreeNode> nod){
    if (!nod)
        return -1;
    return max(calculate_height_aux(nod->right), calculate_height_aux(nod->left)) + 1;
}


void calculate_height_int(std::shared_ptr<RankTreeNode> nod){
    if(nod){
        printf("%d ", nod->key);
        assert( nod->height == calculate_height_aux(nod));
    }
}
void test_BF(std::shared_ptr<RankTreeNode> nod){
    if(nod)
        assert(nod->get_balance_factor()<2 && nod->get_balance_factor() > -2);
}


void test_add(RankTree tree){
    print_tree(tree.root);
    for(int i = 0; i < 15; i++)
        tree.add(i,i);
    print_tree(tree.root);
    tree.do_inorder(calculate_height_int);
    tree.do_inorder(test_BF);
}
void test_add_rl();
void test_add_rr();
void test_add_lr();
void test_add_ll();



int main(){
    int arr1[] = {1,2,3,4,5,6,7,8,9,10,11};
    RankTree tree = test_create_int_int(11);
    test_add(tree);
    tree.print_inorder();
    tree.put_inorder(arr1, arr1, 11);
    std::shared_ptr<RankTreeNode> nd = tree.find(4);
//    nd->left = (RankTree::LR_rotate(nd->left));
    nd = tree.get(11);
    tree.print_inorder();
    for(int i = 0; i< 3; i++)
        tree.add(12+i,12+i);
    tree.print_inorder();
    nd = tree.get(12);
    int i = 0;
}