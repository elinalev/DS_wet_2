#include "avl_tree.h"
#include <assert.h>


int calculate_height_aux(std::shared_ptr<TreeNode<int,int>> nod){
if (!nod)
return -1;
return max(calculate_height_aux(nod->right), calculate_height_aux(nod->left)) + 1;
}


void calculate_height_int(std::shared_ptr<TreeNode<int,int>> nod){
if(nod){
printf("%d ", nod->key);
assert( nod->height == calculate_height_aux(nod));
}
}
void test_BF(std::shared_ptr<TreeNode<int, int>> nod){
if(nod)
assert(nod->get_balance_factor()<2 && nod->get_balance_factor() > -2);
}


int space = 0;
template<class T, class S>
void print_tree(std::shared_ptr<TreeNode<T, S>> node){
    if(node == nullptr)
        return;
    space++;
    print_tree(node->left);
    for (int i = 0; i< space*10; i++)
        printf(" ");
    printf("key=%d value=%d sum=%d size=%d\n", node->key, node->value, node->sum_of_subtree, node->size_of_subtree);
    print_tree(node->right);
    space--;


}


int main(){
    int arr1[] = {1,2,3,4,5,6,7,8,9,10,11};
    int index = 0;
    AvlTree<int, int> tree;
    tree.add(1, 1);
    tree.add(2,2);
    tree.add(3,3);
    tree.root = AvlTree<int, int>::create_half_full_avl_tree(3, 15, 11, arr1, arr1, &index);
    print_tree<int, int>(tree.root);
    tree.add(3,243);
    printf("%d", (tree.get(3))->value);
}