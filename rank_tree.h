#ifndef DS_WET_1_AVL_TREE_H
#define DS_WET_1_AVL_TREE_H

#include <iostream>
#include "rank_tree_node.h"
#include "utils.h"
#include "library2.h"
#define MIN_COUNT 1

class RankTree{
private:

    /// properties
    std::shared_ptr<RankTreeNode> root;
    int* zero;
    int size;

    /// functions
    /// create a half full tree, with keys and values from parameters
    /// \param full_size - size of minimal full tree, beggier then size
    /// \param size - number of nodes in tree
    /// \param key_arr - keys, from smaller to bigger
    /// \param val_arr - values, each one relates to the key of the same index
    /// \param index - int pointer, used among all levels of recursion
    /// \return new half full node
    static std::shared_ptr<RankTreeNode> create_half_full_avl_tree(int full_size, int size, int key_arr[], int val_arr[], int* index);

    // returns the parent of this key (or the place is should be added), or null if it is the root
    std::shared_ptr<RankTreeNode> find(int  key);

    //assists with get_inorder
    StatusType inner_get_inorder(std::shared_ptr<RankTreeNode> node, int number_of_nodes, int  key_arr[],
                                 int val_arr[], int *index) const;

    //assist with add
    std::shared_ptr<RankTreeNode> inner_add(std::shared_ptr<RankTreeNode> upper_node, std::shared_ptr<RankTreeNode> node_to_put);

    //get node nd and return the node to left or right accordingly
    static std::shared_ptr<RankTreeNode> LL_rotate(std::shared_ptr<RankTreeNode> node_to_rotate);
    static std::shared_ptr<RankTreeNode> RR_rotate(std::shared_ptr<RankTreeNode> node_to_rotate);
    static std::shared_ptr<RankTreeNode> LR_rotate(std::shared_ptr<RankTreeNode> node_to_rotate);
    static std::shared_ptr<RankTreeNode> RL_rotate(std::shared_ptr<RankTreeNode> node_to_rotate);


    // inner function for removing nodes from the tree
    void removeLeaf(std::shared_ptr<RankTreeNode>* parent_ptr);
    void removeNodeWithOnlyOneChild(std::shared_ptr<RankTreeNode>* parent_ptr,
                                    std::shared_ptr<RankTreeNode> current);
    void removeNodeWithTwoChildren(std::shared_ptr<RankTreeNode> node_to_remove);
    void removeRoot();
    int inner_get_rank(int key, std::shared_ptr<RankTreeNode> node);
    int inner_get_sum(int key, std::shared_ptr<RankTreeNode> node);
    std::shared_ptr<RankTreeNode> fixBalanceFactor(std::shared_ptr<RankTreeNode> upper_node, int parent_key);

public:
    //todo: remove test fuctions
    int inner_get_height(std::shared_ptr<RankTreeNode> nod){
        if(nod)
            return 1 + max(inner_get_height(nod->left), inner_get_height(nod->right));
        return -1;
    }
    int get_height() {return inner_get_height(root);}
    bool inner_compare_tree_inorder(std::shared_ptr<RankTreeNode> nod, int arr[], int* index){
        if(!nod)
            return true;
        if (!inner_compare_tree_inorder(nod->left, arr, index))
            return false;
        if(nod->value == 0)
            return inner_compare_tree_inorder(nod->right, arr, index);
        if(nod->key != arr[(*index)++])
            return false;
        return inner_compare_tree_inorder(nod->right, arr, index);

        remove(nod->key);
    }
    bool compare_tree_inorder(int arr[]){
        int index = 0;
        return inner_compare_tree_inorder(root, arr, &index);
    }
    void inner_do_inorder(void(fun)(std::shared_ptr<RankTreeNode>), std::shared_ptr<RankTreeNode> nod){
        if(nod){
            inner_do_inorder(fun, nod->left);
            fun(nod);
        }
    }
    void do_inorder(void(fun)(std::shared_ptr<RankTreeNode>)){
        inner_do_inorder(fun, root);
    }

    // getter functions
    int get_size() const{
        return this->size;
    }

    //return the number of values smaller(not equal!) to a specific key
    int get_rank(int key);

    //return the sum of all values with smaller(not equal) keys
    int get_sum(int key);
    static RankTree merge(const RankTree& tree1, const RankTree& tree2);
    // constructors
    RankTree(): root(std::make_shared<RankTreeNode>(0)), zero(&(root->value)), size(0){};

    // basic tree operations
    StatusType add(int  key);
    StatusType add_0();
    std::shared_ptr<RankTreeNode> get(int  key); // returns null if the key isn't in the tree
    StatusType remove( int  key); // does nothing if the key doesn't exists in the tree


    // fill the arrays with correct keys and values
    StatusType get_inorder(int number_of_nodes, int  key_arr[], int val_arr[]) const;

    ///TODO: friends for testing, remove after
    friend int main();
    friend void print_tree(std::shared_ptr<RankTreeNode> node);
    friend void TEST_balanceFactor();
    };






std::shared_ptr<RankTreeNode> RankTree::get(int  key){
    std::shared_ptr<RankTreeNode> parent = find(key);
    if(parent == nullptr)
        return root;

    if(parent->left && parent->left->key == key)
        return parent->left;

    if(parent->right && parent->right->key == key)
        return parent->right;

    return nullptr;

}

int RankTree::inner_get_rank(int key, std::shared_ptr<RankTreeNode> node){
    if(!node)
        return 0;
    if(node-> key > key)
        return inner_get_rank(key, node->left);
    int left_and_val = (node->key != 0 ? node->value : 0) + (node->left? node->left->size_of_subtree : 0);
    return left_and_val + inner_get_rank(key, node->right);
}

int RankTree::get_rank(int key){
    return *zero + inner_get_rank(key, root);
}

int RankTree::inner_get_sum(int key, std::shared_ptr<RankTreeNode> node){
    if(!node)
        return 0;
    if(node-> key >= key)
        return inner_get_sum(key, node->left);
    int left_and_sum = (node->key * node->value) + (node->left? node->left->sum_of_subtree : 0);
    return left_and_sum + inner_get_sum(key, node->right);
}
int RankTree::get_sum(int key){
    return inner_get_sum(key, root);
}

std::shared_ptr<RankTreeNode> RankTree::inner_add(std::shared_ptr<RankTreeNode> upper_node, std::shared_ptr<RankTreeNode> node_to_put) {
    // This is the first node added to the tree
    if(size == 0 && node_to_put != nullptr && upper_node == nullptr){
        return node_to_put;
    }

    if (upper_node == nullptr || node_to_put == nullptr)
        return nullptr;

    // save the node in the right place (no pun intended)
    if (*upper_node < *node_to_put)
        upper_node->right = upper_node->right ? inner_add(upper_node->right, node_to_put) : node_to_put;
    else if(*upper_node == *node_to_put){
        upper_node->value++;
    }
    else
        upper_node->left = upper_node->left ? inner_add(upper_node->left, node_to_put) : node_to_put;

    // check the balance factor and do a roll if necessary
    if (upper_node->get_balance_factor() == 2)
        return upper_node->left->get_balance_factor() >= 0 ? LL_rotate(upper_node) : LR_rotate(upper_node);
    else if (upper_node->get_balance_factor() == -2)
        return upper_node->right->get_balance_factor() <= 0 ? RR_rotate(upper_node) : RL_rotate(upper_node);
    upper_node->update_details();
    return upper_node;
}



int merge_arr(int  key_arr[], int key_arr1[], int  key_arr2[], int val_arr[],
           int val_arr1[], int val_arr2[], int size1, int size2){
    int p1 = 0, p2 = 0, p_new = 0;

    while(p1<size1 && p2<size2) {
        if (key_arr1[p1] == key_arr2[p2]){
            key_arr[p_new] = key_arr1[p1];
            val_arr[p_new++] = val_arr1[p1++] + val_arr2[p2++];
        }
        else if (key_arr1[p1] < key_arr2[p2]) {
            key_arr[p_new] = key_arr1[p1];
            val_arr[p_new++] = val_arr1[p1++];
        } else {
            key_arr[p_new] = key_arr2[p2];
            val_arr[p_new++] = val_arr2[p2++];
        }
    }
    while(p1<size1){
        key_arr[p_new] = key_arr1[p1];
        val_arr[p_new++] = val_arr1[p1++];
    }
    while(p2<size2){
        key_arr[p_new] = key_arr2[p2];
        val_arr[p_new++] = val_arr2[p2++];
    }
    return p_new;
}
RankTree RankTree::merge(const RankTree& tree1, const RankTree& tree2){
    int size1 = tree1.get_size(), size2 = tree2.get_size(), index = 0;
    //we need to copy 0 even if its node does not represent any value
    //therefore not counted in size
    if(*(tree1.zero)==0)
        size1 ++;
    if(*(tree2.zero) == 0)
        size2++;
    int val_arr1[size1], key_arr1[size1], val_arr2[size2], key_arr2[size2], key_arr[size1+size2], val_arr[size1+size2];
    tree1.get_inorder(size1, key_arr1, val_arr1);
    tree2.get_inorder(size2, key_arr2, val_arr2);
    int size = merge_arr(key_arr, key_arr1, key_arr2, val_arr, val_arr1, val_arr2, size1, size2);
    RankTree tree;
    tree.root = create_half_full_avl_tree(find_pow_of_2(size)-1, size, key_arr, val_arr, &index);
    tree.size = size;
    tree.zero = &(tree.get(0))->value;
    return tree;
}


StatusType RankTree::add_0(){
    if(zero){
        (*zero)++;
        size++;
    } else
        return FAILURE;
    return SUCCESS;
}


StatusType RankTree::add(int key){
    if(key == 0){
        return add_0();
    }
    try {
        std::shared_ptr<RankTreeNode> new_node = std::make_shared<RankTreeNode>(key);
        new_node->value = MIN_COUNT;
        new_node->update_details();
        this->root = RankTree::inner_add(root, new_node);
    }catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    size++;
    return SUCCESS;
}



std::shared_ptr<RankTreeNode> RankTree::find(int  key){

    if(this->root == nullptr || this->root->key == key)
        return nullptr;

    std::shared_ptr<RankTreeNode> curr = this->root, next = curr->right;
    if(this->root->key > key)
        next = this->root->left;

    while(next != nullptr){
        if (next->key == key)
            return curr;
        curr = next;
        if (next->key < key)
            next = next->right;
        else if (next->key > key)
            next = next->left;
    }
    return curr;
}


StatusType RankTree::inner_get_inorder(std::shared_ptr<RankTreeNode> node,
                                       int number_of_nodes, int  key_arr[], int val_arr[], int *index) const{

    if(node == nullptr)
        return SUCCESS;

    if(inner_get_inorder(node->left, number_of_nodes, key_arr, val_arr, index) == FAILURE)
        return FAILURE;

    if(number_of_nodes == *index)
        // using the FAILURE flag to stop
        return FAILURE;

    // fill array
    key_arr[*index] = node->key;
    val_arr[*index] = node->value;
    (*index)++;

    if(inner_get_inorder(node->right, number_of_nodes, key_arr, val_arr, index)== FAILURE)
        return FAILURE;

    return SUCCESS;
}


StatusType RankTree::get_inorder(int number_of_nodes, int  key_arr[], int val_arr[]) const{
    int index = 0;
    return inner_get_inorder(this->root, number_of_nodes, key_arr, val_arr, &index);
}




std::shared_ptr<RankTreeNode> RankTree::create_half_full_avl_tree
(int full_size, int size, int key_arr[], int val_arr[], int* index){
    std::shared_ptr<RankTreeNode> new_node = std::make_shared<RankTreeNode>(-1);
    if(size == 0)
        return nullptr;
    if(size <= 1){
        new_node->height = 0;
        new_node->key = key_arr[*index];
        new_node->value = val_arr[(*index)++];
        new_node->update_details();
        return new_node;
    }
    int size_of_left, full_size_of_right;
    int full_son/*without leaves*/ =(((full_size-1)/2)-1)/2, leaves/*of full son*/ = full_son + 1;
    if(2 * full_son + leaves < size - 1){
        size_of_left = full_son + leaves;
        full_size_of_right = (full_size -1)/2;
    }
    else if(2 * full_son + leaves == size - 1){
        size_of_left = full_son + leaves;
        full_size_of_right = full_son;
    }
    else{
        size_of_left = full_son + 2*leaves - (full_size - size);
        full_size_of_right = full_son;
    }

    new_node->left = RankTree::create_half_full_avl_tree((full_size - 1) / 2,
                                                         size_of_left, key_arr, val_arr, index);
    new_node->key = key_arr[*index];
    new_node->value = val_arr[(*index)++];
    new_node->right = RankTree::create_half_full_avl_tree(full_size_of_right,
                                                               size-1-size_of_left, key_arr, val_arr, index);

    new_node->update_details();

    return new_node;
}





std::shared_ptr<RankTreeNode> RankTree::LL_rotate(std::shared_ptr<RankTreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->left == nullptr)
        return nullptr;
    std::shared_ptr<RankTreeNode> left = node_to_rotate->left;
    node_to_rotate->left = left->right;
    left->right = node_to_rotate;
    node_to_rotate->update_details();
    left->update_details();
    return left;
}


std::shared_ptr<RankTreeNode> RankTree::RR_rotate(std::shared_ptr<RankTreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->right == nullptr)
        return nullptr;
    std::shared_ptr<RankTreeNode> right = node_to_rotate->right;
    node_to_rotate->right = right->left;
    right->left = node_to_rotate;
    node_to_rotate->update_details();
    right->update_details();
    return right;
}


std::shared_ptr<RankTreeNode> RankTree::RL_rotate(std::shared_ptr<RankTreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->right == nullptr||node_to_rotate->right->left == nullptr)
        return nullptr;
    node_to_rotate->right = LL_rotate(node_to_rotate->right);
    return RR_rotate(node_to_rotate);
}


std::shared_ptr<RankTreeNode> RankTree::LR_rotate(std::shared_ptr<RankTreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->left == nullptr||node_to_rotate->left->right == nullptr)
        return nullptr;
    node_to_rotate->left = RR_rotate(node_to_rotate->left);
    return LL_rotate(node_to_rotate);
}


void RankTree::removeLeaf(std::shared_ptr<RankTreeNode>* parent_ptr)
{
    (*parent_ptr) = nullptr;
    size--;
}


void RankTree::removeNodeWithOnlyOneChild(std::shared_ptr<RankTreeNode>* parent_ptr,
                                          std::shared_ptr<RankTreeNode> current){
    // left child exists
    if(!(current->right) && current->left)
    {
        (*parent_ptr) = current->left;
        size--;
        return;
    }
    // right child exists
    if(!(current->left) && current->right)
    {
        (*parent_ptr) = current->right;
        size--;
        return;
    }
}



void RankTree::removeNodeWithTwoChildren(std::shared_ptr<RankTreeNode> node_to_remove){
    // step 1: find the replacement node (go right and then all the way left)
    auto replacement_node = node_to_remove->right;
    auto replacement_node_parent = node_to_remove;

    while(replacement_node->left){
        replacement_node_parent = replacement_node;
        replacement_node = replacement_node->left;
    }

    // step 2: put the replacement node instead of current
    node_to_remove->key = replacement_node->key;
    node_to_remove->value = replacement_node->value;

    // step 3: remove the replacement_node from its original place
    std::shared_ptr<RankTreeNode> *replacement_node_parent_ptr;
    if(replacement_node_parent->right && (replacement_node_parent->right->key == replacement_node->key))
    {
        replacement_node_parent_ptr = &(replacement_node_parent->right);
    }
    else{
        replacement_node_parent_ptr = &(replacement_node_parent->left);
    }

    if(!(replacement_node->right) && !(replacement_node->left)) {
        removeLeaf(replacement_node_parent_ptr);
    }
    else{
        removeNodeWithOnlyOneChild(replacement_node_parent_ptr, replacement_node);
    }
}



void RankTree::removeRoot() {
    if(!(root->right) && !(root->left)){
        root = nullptr;
        size--;
    }
    else if(root->right && root->left){
        removeNodeWithTwoChildren(root);
    }
    else{
        if(root->left)
        {
            root = root->left;
        }
        else{
            root = root->right;
        }
        size--;
    }
}


std::shared_ptr<RankTreeNode> RankTree::fixBalanceFactor(std::shared_ptr<RankTreeNode> upper_node, int parent_key)
{
    // recurse until we find the parent
    if (upper_node.get()->key < parent_key)
    {
        upper_node->right = fixBalanceFactor(upper_node->right, parent_key);
    }
    else if (upper_node.get()->key > parent_key)
    {
        upper_node->left = fixBalanceFactor(upper_node->left, parent_key);
    }

    if (upper_node->get_balance_factor() == 2)
    {
        return upper_node->left->get_balance_factor() >= 0 ? LL_rotate(upper_node) : LR_rotate(upper_node);
    }
    else if (upper_node->get_balance_factor() == -2)
    {
        return upper_node->right->get_balance_factor() <= 0 ? RR_rotate(upper_node) : RL_rotate(upper_node);
    }

    // update the node height
    upper_node->height = max(upper_node->right? upper_node->right->height: -1,
                             upper_node->left? upper_node->left->height : -1) + 1;

    return upper_node;
}



StatusType RankTree::remove(int key){
    std::shared_ptr<RankTreeNode> current = get(key);
    if(current == nullptr){
        return FAILURE;
    }
    else if(current->value >= 2){
        current->value--;
        root = fixBalanceFactor(root, key);
        return SUCCESS;
    }
    else if(current->key == 0){
        if(current->value == 1){
            current->value--;
            return SUCCESS;
        }
        return FAILURE;
    }
    std::shared_ptr<RankTreeNode> parent = find(key);
    std::shared_ptr<RankTreeNode>* parent_ptr;

    if(parent == nullptr){
        // no need to fix the tree if we removed the root
        removeRoot();
        return SUCCESS;
    }
    else if(parent->right && parent->right->key == key){
        current = parent->right;
        parent_ptr= &(parent->right);
    }
    else if(parent->left && parent->left->key == key){
        current = parent->left;
        parent_ptr= &(parent->left);
    }
    // Note: if the node doesn't exists, we will do nothing

    // remove the node according to the number of children
    if(!(current->right) && !(current->left)) {
        removeLeaf(parent_ptr);
    }
    else if (current->right && current->left){
        removeNodeWithTwoChildren(current);
    }
    else{
        removeNodeWithOnlyOneChild(parent_ptr, current);
    }

    root = fixBalanceFactor(root, parent.get()->key);
    return SUCCESS;
}
/*
StatusType RankTree::reduce_val(int key){
    std::shared_ptr<RankTreeNode> nod = get(key);
    if(nod == nullptr)
        return INVALID_INPUT;
    else if(nod->key == 0){
        if(nod->value <= 0)
            return INVALID_INPUT;
        nod->value--;
        return SUCCESS;
    }
    nod->value--;
    if(nod->value <= 0)
        return remove(key);
    return SUCCESS;
}*/

#endif //DS_WET_1_AVL_TREE_H