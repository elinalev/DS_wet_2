#ifndef DS_WET_1_AVL_TREE_H
#define DS_WET_1_AVL_TREE_H

#include "tree_node.h"
#include "utils.h"
#include "library2.h"


class AvlTree{
private:

    /// properties
    std::shared_ptr<TreeNode> root;
    std::shared_ptr<int>  zero;
    int size;

    /// functions
    // creates a full avl tree of size with empty keys
    static std::shared_ptr<TreeNode> create_half_full_avl_tree(int full_size, int size, int key_arr[], int val_arr[], int* index);
    // returns the parent of this key (or the place is should be added), or null if it is the root
    std::shared_ptr<TreeNode> find( int  key);

    StatusType inner_get_inorder(std::shared_ptr<TreeNode> node, int number_of_nodes, int  key_arr[],
                                 int val_arr[], int *index) const;

    // using inner_add assumes the node is not in the tree
    std::shared_ptr<TreeNode> inner_add(std::shared_ptr<TreeNode> upper_node, std::shared_ptr<TreeNode> node_to_put);

    //get node nd and return the node to left or right accordingly
    static std::shared_ptr<TreeNode> LL_rotate(std::shared_ptr<TreeNode> node_to_rotate);
    static std::shared_ptr<TreeNode> RR_rotate(std::shared_ptr<TreeNode> node_to_rotate);
    static std::shared_ptr<TreeNode> LR_rotate(std::shared_ptr<TreeNode> node_to_rotate);
    static std::shared_ptr<TreeNode> RL_rotate(std::shared_ptr<TreeNode> node_to_rotate);
    int innerGetMax(std::shared_ptr<TreeNode>);


    // inner function for removing nodes from the tree
    void removeLeaf(std::shared_ptr<TreeNode>* parent_ptr);
    void removeNodeWithOnlyOneChild(std::shared_ptr<TreeNode>* parent_ptr,
                                    std::shared_ptr<TreeNode> current);
    void removeNodeWithTwoChildren(std::shared_ptr<TreeNode> node_to_remove);
    void removeRoot();

    std::shared_ptr<TreeNode> fixBalanceFactor(std::shared_ptr<TreeNode> upper_node, int parent_key);

public:

    // getter functions
    int get_size() const{
        return this->size;
    }
    int get_max();
    static AvlTree merge(const AvlTree& tree1, const AvlTree& tree2);
    // constructors
    AvlTree():root(std::make_shared<TreeNode>(0)),zero(std::make_shared<int>(root->value)), size(0){};
    // basic tree operations
    StatusType add( int  key, int value);
    StatusType add_0();
    std::shared_ptr<TreeNode> get( int  key); // returns null if the key isn't in the tree
    StatusType remove( int  key); // does nothing if the key doesn't exists in the tree
    //use that function to reduce the counter of a node, will remove it if gets empty.
    StatusType reduce_val(int key);
    bool is_empty();

    // inorder functions
    StatusType get_inorder(int number_of_nodes, int  key_arr[], int val_arr[]) const;
    friend int main();
    friend void print_tree(std::shared_ptr<TreeNode> node);
};



bool AvlTree::is_empty(){
    return (root == nullptr) || (size == 0);
}

int  AvlTree::get_max(){
    return innerGetMax(root);
}


int  AvlTree::innerGetMax(std::shared_ptr<TreeNode> nod){
    if(nod == nullptr || nod ->right == nullptr)
        return nod->value;
    return innerGetMax(nod->right);
}


std::shared_ptr<TreeNode> AvlTree::get( int  key){
    std::shared_ptr<TreeNode> parent = find(key);
    if(parent == nullptr)
        return root;

    if(parent->left && parent->left->key == key)
        return parent->left;

    if(parent->right && parent->right->key == key)
        return parent->right;

    return nullptr;

}


std::shared_ptr<TreeNode> AvlTree::inner_add(std::shared_ptr<TreeNode> upper_node, std::shared_ptr<TreeNode> node_to_put) {
    // This is the first node added to the tree
    if(size == 0 && node_to_put != nullptr && upper_node == nullptr){
        return node_to_put;
    }

    if (upper_node == nullptr || node_to_put == nullptr)
        return nullptr;

    // save the node in the right place (no pun intended)
    if (*upper_node < *node_to_put)
        upper_node->right = upper_node->right ? inner_add(upper_node->right, node_to_put) : node_to_put;
    else
        upper_node->left = upper_node->left ? inner_add(upper_node->left, node_to_put) : node_to_put;

    // check the balance factor and do a roll if necessary
    if (upper_node->get_balance_factor() == 2)
        return upper_node->left->get_balance_factor() >= 0 ? LL_rotate(upper_node) : LR_rotate(upper_node);
    else if (upper_node->get_balance_factor() == -2)
        return upper_node->right->get_balance_factor() <= 0 ? RR_rotate(upper_node) : RL_rotate(upper_node);

    // update the node height
    upper_node->height = max(upper_node->right? upper_node->right->height: -1,
                             upper_node->left? upper_node->left->height : -1) + 1;
    upper_node->size_of_subtree = (upper_node->right? upper_node->right->size_of_subtree : 0)
            + (upper_node->left? upper_node->left->size_of_subtree : 0) + 1;
    upper_node->sum_of_subtree = (upper_node->right? upper_node->right->sum_of_subtree : 0)
                                  + (upper_node->left? upper_node->left->sum_of_subtree : 0) + ((upper_node->key) * (upper_node->value));
    return upper_node;
}



int merge_arr(int  key_arr[], int key_arr1[], int  key_arr2[], int val_arr[],
           int val_arr1[], int val_arr2[], int size1, int size2){
    int p1 = 0, p2 = 0, p_new = 0, new_size = 0;
    while(p1<size1 && p2<size2) {
        new_size++;
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
        new_size++;
        key_arr[p_new] = key_arr1[p1];
        val_arr[p_new++] = val_arr1[p1++];
    }
    while(p2<size2){
        new_size++;
        key_arr[p_new] = key_arr2[p2];
        val_arr[p_new++] = val_arr2[p2++];
    }
    return new_size;
}
AvlTree AvlTree::merge(const AvlTree& tree1, const AvlTree& tree2){
    int size1 = tree1.get_size(), size2 = tree2.get_size(), index = 0;
    int val_arr1[size1], key_arr1[size1], val_arr2[size2], key_arr2[size2], key_arr[size1+size2], val_arr[size1+size2];
    StatusType status;
    status = tree1.get_inorder(size1, key_arr1, val_arr1);
    tree2.get_inorder(size2, key_arr2, val_arr2);
    int size = merge_arr(key_arr, key_arr1, key_arr2, val_arr, val_arr1, val_arr2, size1, size2);

    AvlTree tree;
    tree.root = create_half_full_avl_tree(find_pow_of_2(size)-1, size, key_arr, val_arr, &index);
    tree.size = size;
    tree.zero = std::make_shared<int>((tree.get(0))->value);
    return tree;
}


StatusType AvlTree::add_0(){
    if(zero){
        (*zero)++;
    } else
        return FAILURE;
    return SUCCESS;
}
/*

StatusType AvlTree::add( int  key, int value){
    TreeNode right_one = this->get(key);
    if(right_one != nullptr) {
        right_one.value++;
        return SUCCESS;
    }
    try {
        std::shared_ptr<TreeNode> new_node = std::make_shared<TreeNode>();
        new_node->key = key;
        new_node->value = value;
        this->root = AvlTree::inner_add(root, new_node);
    }catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    size++;
    return SUCCESS;
}*/



std::shared_ptr<TreeNode> AvlTree::find( int  key){

    if(this->root == nullptr || this->root->key == key)
        return nullptr;

    std::shared_ptr<TreeNode> curr = this->root, next = curr->right;
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


StatusType AvlTree::inner_get_inorder(std::shared_ptr<TreeNode> node,
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


StatusType AvlTree::get_inorder(int number_of_nodes, int  key_arr[], int val_arr[]) const{
    int index = 0;
    return inner_get_inorder(this->root, number_of_nodes, key_arr, val_arr, &index);
}




std::shared_ptr<TreeNode> AvlTree::create_half_full_avl_tree
(int full_size, int size, int key_arr[], int val_arr[], int* index){
    std::shared_ptr<TreeNode> new_node = std::make_shared<TreeNode>(-1);
    if(size == 0)
        return nullptr;
    if(size <= 1){
        new_node->height = 0;
        new_node->sum_of_subtree = key_arr[*index] * val_arr[*index];
        new_node->key = key_arr[*index];
        new_node->value = val_arr[(*index)++];
        new_node->size_of_subtree = new_node->value;
        return new_node;
    }
    int size_of_left, full_size_of_right;
    int full_son/*without leaves*/ =(((full_size-1)/2)-1)/2, leaves/*of full son*/ = full_son + 1;
    if(2 * full_son + leaves < size){
        size_of_left = full_son + leaves;
        full_size_of_right = (full_size -1)/2;
    }
    else if(2 * full_son + leaves == size){
        size_of_left = full_son + leaves;
        full_size_of_right = full_son;
    }
    else{
        size_of_left = full_son + 2*leaves - (full_size - size);
        full_size_of_right = full_son;
    }

    new_node->left = AvlTree::create_half_full_avl_tree((full_size - 1) / 2,
                                                              size_of_left, key_arr, val_arr, index);
    new_node->key = key_arr[*index];
    new_node->value = val_arr[(*index)++];
    new_node->right = AvlTree::create_half_full_avl_tree(full_size_of_right,
                                                               size-1-size_of_left, key_arr, val_arr, index);

    int left_height = new_node->left? new_node->left->height : -1;
    int right_height = new_node->right? new_node->right->height : -1;
    new_node->height = max(left_height, right_height) + 1;

    int left_size = new_node->left? new_node->left->size_of_subtree : 0;
    int right_size = new_node->right? new_node->right->size_of_subtree : 0;
    new_node->size_of_subtree = left_size + right_size + new_node->value;

    int left_sum = new_node->left? new_node->left->sum_of_subtree : 0;
    int right_sum = new_node->right? new_node->right->sum_of_subtree : 0;
    new_node->sum_of_subtree = left_sum + right_sum + ((new_node->key) * (new_node->value));
    return new_node;
}





std::shared_ptr<TreeNode> AvlTree::LL_rotate(std::shared_ptr<TreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->left == nullptr)
        return nullptr;
    std::shared_ptr<TreeNode> left = node_to_rotate->left;
    node_to_rotate->left = left->right;
    left->right = node_to_rotate;
    node_to_rotate->height = node_to_rotate->cal_height_non_recursive();
    left->height = left->cal_height_non_recursive();
    return left;
}


std::shared_ptr<TreeNode> AvlTree::RR_rotate(std::shared_ptr<TreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->right == nullptr)
        return nullptr;
    std::shared_ptr<TreeNode> right = node_to_rotate->right;
    node_to_rotate->right = right->left;
    right->left = node_to_rotate;
    node_to_rotate->height = node_to_rotate->cal_height_non_recursive();
    right->height = right->cal_height_non_recursive();
    return right;
}


std::shared_ptr<TreeNode> AvlTree::RL_rotate(std::shared_ptr<TreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->right == nullptr||node_to_rotate->right->left == nullptr)
        return nullptr;
    node_to_rotate->right = LL_rotate(node_to_rotate->right);
    return RR_rotate(node_to_rotate);
}


std::shared_ptr<TreeNode> AvlTree::LR_rotate(std::shared_ptr<TreeNode> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->left == nullptr||node_to_rotate->left->right == nullptr)
        return nullptr;
    node_to_rotate->left = RR_rotate(node_to_rotate->left);
    return LL_rotate(node_to_rotate);
}


void AvlTree::removeLeaf(std::shared_ptr<TreeNode>* parent_ptr)
{
    (*parent_ptr) = nullptr;
    size--;
}


void AvlTree::removeNodeWithOnlyOneChild(std::shared_ptr<TreeNode>* parent_ptr,
                                              std::shared_ptr<TreeNode> current){
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



void AvlTree::removeNodeWithTwoChildren(std::shared_ptr<TreeNode> node_to_remove){
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
    std::shared_ptr<TreeNode> *replacement_node_parent_ptr;
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



void AvlTree::removeRoot() {
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


std::shared_ptr<TreeNode> AvlTree::fixBalanceFactor(std::shared_ptr<TreeNode> upper_node, int parent_key)
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



StatusType AvlTree::remove(int key){
    std::shared_ptr<TreeNode> current = get(key);
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
    std::shared_ptr<TreeNode> parent = find(key);
    std::shared_ptr<TreeNode>* parent_ptr;

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

StatusType AvlTree::reduce_val(int key){
    std::shared_ptr<TreeNode> nod = get(key);
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
}

#endif //DS_WET_1_AVL_TREE_H