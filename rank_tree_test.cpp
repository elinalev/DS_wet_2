#include "rank_tree.h"
#include <assert.h>


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


int space = 0;
void print_tree(std::shared_ptr<RankTreeNode> node){
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
    int arr1[] = {0, 1,2,3,4,5,6,7,8,9,10,11, 12, 13, 14};
    int arr2[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1 ,1};
    int index = 0;
    int size = 12;
    int full_size = find_pow_of_2(size) -1;
    RankTree tree;
    tree.root = RankTree::create_half_full_avl_tree(full_size, size, arr1, arr2, &index);
    tree.size = size;
    RankTree mtree = RankTree::merge(tree, tree);
    print_tree(mtree.root); space = 5;

    for(int i = 0; i<7; i++){
        mtree.remove(i+1);
        mtree.remove(0);

    }


    print_tree(mtree.root); space = 0;
}