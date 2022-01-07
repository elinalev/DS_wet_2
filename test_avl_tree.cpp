#include "rank_tree.h"
#include <iostream>
#include <assert.h>


/* helper function for tests */

void test_BF(std::shared_ptr<RankTreeNode> nod){
    if(nod)
        assert(nod->get_balance_factor()<2 && nod->get_balance_factor() > -2);
}

void test_Value(std::shared_ptr<RankTreeNode> nod, int value){
    if(nod)
        assert(nod->value == value);
}

/* tests */

void TEST_CreateEmptyTree() {
    std::cout << "Running TEST_CreateEmptyTree" << std::endl;
    RankTree tree = RankTree();

    assert(tree.get_size() == 0 && "TEST_CreateEmptyTree failed");
    std::cout << "TEST_CreateEmptyTree succeeded" << std::endl;
}

void TEST_AddOneToTree() {
    std::cout << "Running TEST_AddOneToTree" << std::endl;
    RankTree tree = RankTree();
    tree.add(1);

    assert(tree.get_size() == 1 && "TEST_AddOneToTree failed");

    auto node = tree.get(1);
    assert(node != nullptr && "TEST_AddOneToTree failed");
    assert(node->value == 1 && "TEST_AddOneToTree failed");

    std::cout << "TEST_AddOneToTree succeeded" << std::endl;
}

void TEST_GetFromEmptyTree() {
    std::cout << "Running TEST_GetFromEmptyTree" << std::endl;
    RankTree tree = RankTree();
    auto node = tree.get(0);
    if(node == nullptr){
        std::cout << "TEST_GetFromEmptyTree succeeded" << std::endl;
    }
    else{
        std::cout << "TEST_GetFromEmptyTree failed" << std::endl;
    }
}

void TEST_AddRRToTree(){
    std::cout << "Running TEST_AddRRToTree" << std::endl;
    RankTree tree = RankTree();
    tree.add(1);
    tree.add(2);
    tree.add(3);

    tree.do_inorder(test_BF);
    std::cout << "TEST_AddRRToTree succeeded" << std::endl;
}

void TEST_AddLLToTree(){
    std::cout << "Running TEST_AddLLToTree" << std::endl;
    RankTree tree = RankTree();
    tree.add(3);
    tree.add(2);
    tree.add(1);

    tree.do_inorder(test_BF);
    std::cout << "TEST_AddLLToTree succeeded" << std::endl;
}

void TEST_AddRLToTree(){
    std::cout << "Running TEST_AddRLToTree" << std::endl;
    RankTree tree = RankTree();
    tree.add(1);
    tree.add(3);
    tree.add(2);

    tree.do_inorder(test_BF);
    std::cout << "TEST_AddRLToTree succeeded" << std::endl;
}

void TEST_AddLRToTree(){
    std::cout << "Running TEST_AddLRToTree" << std::endl;
    RankTree tree = RankTree();
    tree.add(3);
    tree.add(1);
    tree.add(2);

    tree.do_inorder(test_BF);
    std::cout << "TEST_AddLRToTree succeeded" << std::endl;
}

/*void TEST_CreateFullTree(int tree_size){
    std::cout << "Running TEST_CreateFullTree" << std::endl;
    RankTree tree = RankTree.(tree_size);

    assert(tree.get_size() == tree_size && "TEST_CreateFullTree failed");
    std::cout << "TEST_CreateFullTree succeeded" << std::endl;

}*/

void TEST_RemoveLeafFromTree(){
    std::cout << "Running TEST_RemoveLeafFromTree" << std::endl;
    RankTree tree = RankTree();
    for (int i = 1; i <= 3; ++i) {
        tree.add(i);
    }
    tree.remove(1);

    assert(tree.get(1) == nullptr && "TEST_RemoveLeafFromTree failed");
    std::cout << "TEST_RemoveLeafFromTree succeeded" << std::endl;
}

void TEST_RemoveNodeWithOnechildFromTree(){
    std::cout << "Running TEST_RemoveNodeWithOnechildFromTree" << std::endl;
    RankTree tree = RankTree();
    for (int i = 1; i <= 6; ++i) {
        tree.add(i);
    }
    tree.remove(5);
    assert(tree.get(5) == nullptr && "TEST_RemoveNodeWithOnechildFromTree failed");

    int arr[]={1,2,3,4,6};
    assert(tree.compare_tree_inorder(arr) && "TEST_RemoveNodeWithOnechildFromTree failed");
    std::cout << "TEST_RemoveNodeWithOnechildFromTree succeeded" << std::endl;
}

void TEST_RemoveNodeWithTwochildsFromTree(){
    std::cout << "Running TEST_RemoveNodeWithTwochildsFromTree" << std::endl;
    RankTree tree = RankTree();
    for (int i = 1; i <= 5; ++i) {
        tree.add(i);
    }
    tree.remove(4);
    assert(tree.get(4) == nullptr && "TEST_RemoveNodeWithTwochildsFromTree failed");
    int arr[]={1,2,3,5};
    assert(tree.compare_tree_inorder(arr) && "TEST_RemoveNodeWithTwochildsFromTree failed");

    RankTree tree2 = RankTree();
    for (int i = 1; i <= 9; ++i) {
        tree2.add(i);
    }
    tree2.remove(6);
    assert(tree2.get(6) == nullptr && "TEST_RemoveNodeWithTwochildsFromTree failed");
    int arr2[]={1,2,3,4,5,7,8,9};
    assert(tree2.compare_tree_inorder(arr2) && "TEST_RemoveNodeWithTwochildsFromTree failed");

    RankTree tree3 = RankTree();
    for (int i = 1; i <= 10; ++i) {
        tree3.add(i);
    }
    tree3.remove(8);
    assert(tree3.get(8) == nullptr && "TEST_RemoveNodeWithTwochildsFromTree failed");
    int arr3[]={1,2,3,4,5,6,7,9,10};
    assert(tree3.compare_tree_inorder(arr3) && "TEST_RemoveNodeWithTwochildsFromTree failed");

    std::cout << "TEST_RemoveNodeWithTwochildsFromTree succeeded" << std::endl;
}

void TEST_RemoveRootFromTree(){
    std::cout << "Running TEST_RemoveRootFromTree" << std::endl;
    RankTree tree = RankTree();
    for (int i = 1; i <= 3; ++i) {
        tree.add(i);
    }
    tree.remove(2);
    assert(tree.get(2) == nullptr && "TEST_RemoveRootFromTree failed");
    int arr[]={1,3};
    assert(tree.compare_tree_inorder(arr) && "TEST_RemoveRootFromTree failed");

    std::cout << "TEST_RemoveRootFromTree succeeded" << std::endl;
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

void TEST_balanceFactor(){
    std::cout << "Running TEST_balanceFactor" << std::endl;
    RankTree tree = RankTree();
    int arr[]={5,2,8,1,10,4,6,3,7,9,12,11};
    int counter = 0;
    for (int i = 0; i < 12; ++i) {
        tree.add(arr[i]);
        counter += arr[i];
        assert(tree.get_rank(20) == i+1);
        assert(tree.get_sum(20) == counter);
    }

    tree.remove(1);
    assert(tree.get(1) == nullptr && "TEST_balanceFactor failed");
    assert(tree.get_height() == 4 && "TEST_balanceFactor failed");
    RankTree t2 = RankTree::merge(tree, tree);
    RankTree t3 = RankTree::merge(tree, t2);
    print_tree(tree.root);
    std::cout<< "\n\n\n";
    print_tree(t2.root);
    assert(t2.get_height() == 3 && "TEST_balanceFactor failed");

    std::cout << "TEST_balanceFactor succeeded" << std::endl;
}

void TEST_getInorder(int tree_size){
    std::cout << "Running TEST_getInorder" << std::endl;
    RankTree tree = RankTree();
    int* arr = new int[tree_size];
    for(int i = 0; i < tree_size; i ++){
        arr[i] = i;
        tree.add(arr[i]);
    }

    int* keys = new int[tree_size];
    int* vals = new int[tree_size];

    tree.get_inorder(tree_size, keys, vals);
    for(int i = 0; i < tree_size; i ++){
        assert(keys[i] == i && "TEST_getInorder failed");
    }

    std::cout << "TEST_getInorder succeeded" << std::endl;

    delete[] keys;
    delete[] vals;
}

void TEST_getInorderOnlyHalf(int tree_size){
    std::cout << "Running TEST_getInorderOnlyHalf" << std::endl;
    RankTree tree = RankTree();
    int* arr = new int[tree_size*2];
    for(int i = 0; i < tree_size*2; i ++){
        arr[i] = i;
        tree.add(arr[i]);
    }

    assert(tree.get_size() == tree_size*2 && "TEST_getInorderOnlyHalf failed");

    int* keys = new int[tree_size];
    int* vals = new int[tree_size];

    tree.get_inorder(tree_size, keys, vals);
    for(int i = 0; i < tree_size; i ++){
        assert(keys[i] == i && "TEST_getInorderOnlyHalf failed");
    }

    std::cout << "TEST_getInorderOnlyHalf succeeded" << std::endl;

    delete[] keys;
    delete[] vals;
}

/*
void TEST_CreateAndFillFullTree(int tree_size) {
    std::cout << "Running TEST_CreateAndFillFullTree" << std::endl;
    RankTree tree = RankTree(tree_size);

    int* arr = new int[tree_size];
    for(int i = 0; i < tree_size; i ++){
        arr[i] = i;
    }
    tree.put_inorder(arr, arr, tree_size);

    for(int i = 0; i < tree_size; i ++){
        auto node = tree.get_inorder();
        assert(node != nullptr &&  node->value == i && "TEST_CreateAndFillFullTree failed");
    }

    std::cout << "TEST_CreateAndFillFullTree succeeded" << std::endl;
}
 */

int main(){
    TEST_CreateEmptyTree();
    std::cout << std::endl;

    TEST_AddOneToTree();
    std::cout << std::endl;

    TEST_GetFromEmptyTree();
    std::cout << std::endl;

    TEST_AddRRToTree();
    std::cout << std::endl;

    TEST_AddLLToTree();
    std::cout << std::endl;

    TEST_AddRLToTree();
    std::cout << std::endl;

    TEST_AddLRToTree();
    std::cout << std::endl;

/*
    TEST_CreateFullTree(100);
*/
    std::cout << std::endl;

    TEST_RemoveLeafFromTree();
    std::cout << std::endl;

    TEST_RemoveNodeWithOnechildFromTree();
    std::cout << std::endl;

    TEST_RemoveNodeWithTwochildsFromTree();
    std::cout << std::endl;

    TEST_RemoveRootFromTree();
    std::cout << std::endl;

    TEST_balanceFactor();
    std::cout << std::endl;

    TEST_getInorder(5);
    std::cout << std::endl;

    TEST_getInorderOnlyHalf(5);
    std::cout << std::endl;

    return 0;
}