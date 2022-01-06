#ifndef DS_WET_2_AVL_TREE_H
#define DS_WET_2_AVL_TREE_H

#include "tree_node.h"
#include "utils.h"
#include "library2.h"


template<class T, class S>
class AvlTree{
private:

    /// properties
    std::shared_ptr<TreeNode<T,S>> root;
    int size;

    /// functions
    // creates a full avl tree of size with empty keys
    static std::shared_ptr<TreeNode<T,S>> create_full_avl_tree(int size);
    // returns the parent of this key (or the place is should be added), or null if it is the root
    std::shared_ptr<TreeNode<T,S>> find(T key);

    StatusType inner_get_inorder(std::shared_ptr<TreeNode<T,S>> node, int number_of_nodes,T key_arr[],
                                 S val_arr[], int *index);
    StatusType inner_put_inorder(std::shared_ptr<TreeNode<T,S>> nod, T key_arr[], S value_arr[],
                                 std::shared_ptr<int> min_index, int max_index);

    // using inner_add assumes the node is not in the tree
    std::shared_ptr<TreeNode<T,S>> inner_add(std::shared_ptr<TreeNode<T,S>> upper_node, std::shared_ptr<TreeNode<T,S>> node_to_put);

    //get node nd and return the node to left or right accordingly
    static std::shared_ptr<TreeNode<T,S>> LL_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate);
    static std::shared_ptr<TreeNode<T,S>> RR_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate);
    static std::shared_ptr<TreeNode<T,S>> LR_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate);
    static std::shared_ptr<TreeNode<T,S>> RL_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate);
    S innerGetMax(std::shared_ptr<TreeNode<T,S>>);


    // inner function for removing nodes from the tree
    void removeLeaf(std::shared_ptr<TreeNode<T,S>>* parent_ptr);
    void removeNodeWithOnlyOneChild(std::shared_ptr<TreeNode<T,S>>* parent_ptr,
                                    std::shared_ptr<TreeNode<T,S>> current);
    void removeNodeWithTwoChildren(std::shared_ptr<TreeNode<T,S>> node_to_remove);
    void removeRoot();

    std::shared_ptr<TreeNode<T,S>> fixBalanceFactor(std::shared_ptr<TreeNode<T,S>> upper_node, T parent_key);

public:

    // getter functions
    int get_size(){
        return this->size;
    }
    S get_max();

    // constructors
    AvlTree():root(nullptr), size(0){};
    explicit AvlTree(int number_of_nodes);

    // basic tree operations
    StatusType add(T key, S value);
    std::shared_ptr<TreeNode<T,S>> get(T key); // returns null if the key isn't in the tree
    StatusType remove(T key); // does nothing if the key doesn't exists in the tree
    bool is_empty();

    // inorder functions
    StatusType get_inorder(int number_of_nodes,T key_arr[], S val_arr[]);
    StatusType put_inorder(T key_arr[], S value_arr[], int arr_size);

};


template<class T, class S>
bool AvlTree<T,S>::is_empty(){
    return (root == nullptr) || (size == 0);
}

template<class T , class S>
S AvlTree<T,S>::get_max(){
    return innerGetMax(root);
}

template<class T, class S>
S AvlTree<T,S>::innerGetMax(std::shared_ptr<TreeNode<T,S>> nod){
    if(nod == nullptr || nod ->right == nullptr)
        return nod->value;
    return innerGetMax(nod->right);
}

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::get(T key){
    std::shared_ptr<TreeNode<T,S>> parent = find(key);
    if(parent == nullptr)
        return root;

    if(parent->left && parent->left->key == key)
        return parent->left;

    if(parent->right && parent->right->key == key)
        return parent->right;

    return nullptr;

}

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::inner_add(std::shared_ptr<TreeNode<T,S>> upper_node, std::shared_ptr<TreeNode<T,S>> node_to_put) {
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
    return upper_node;
}

template<class T, class S>
StatusType AvlTree<T,S>::add(T key, S value){
    if(this->get(key) != nullptr)
        return FAILURE;
    try {
        std::shared_ptr<TreeNode<T, S>> new_node = std::make_shared<TreeNode<T, S>>();
        new_node->key = key;
        new_node->value = value;
        this->root = AvlTree<T, S>::inner_add(root, new_node);
    }catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    size++;
    return SUCCESS;
}


template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::find(T key){

    if(this->root == nullptr || this->root->key == key)
        return nullptr;

    std::shared_ptr<TreeNode<T, S>> curr = this->root, next = curr->right;
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

template<class T, class S>
StatusType AvlTree<T,S>::inner_get_inorder(std::shared_ptr<TreeNode<T,S>> node,
                                           int number_of_nodes,T key_arr[], S val_arr[], int *index){

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

template<class T, class S>
StatusType AvlTree<T,S>::get_inorder(int number_of_nodes,T key_arr[], S val_arr[]){
    int index = 0;
    return inner_get_inorder(this->root, number_of_nodes, key_arr, val_arr, &index);
}

template<class T, class S>
StatusType AvlTree<T,S>::put_inorder(T key_arr[], S value_arr[], int arr_size){
    try {
        std::shared_ptr<int> first_index = std::make_shared<int>(0);
        inner_put_inorder(this->root, key_arr, value_arr, first_index, arr_size - 1);
    }catch (const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


template<class T,class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::create_full_avl_tree(int size){
    std::shared_ptr<TreeNode<T,S>> new_node = std::make_shared<TreeNode<T,S>>();
    if(size <= 1){
        new_node->height = 0;
        return new_node;
    }
    new_node->left = AvlTree<T,S>::create_full_avl_tree((size-1)/2);
    new_node->right = AvlTree<T,S>::create_full_avl_tree((size-1)/2);
    new_node->height = max(new_node->left->height, new_node->right->height) + 1;
    return new_node;
}


template<class T, class S>
void remove_while_half_full(std::shared_ptr<TreeNode<T,S>> curr,std::shared_ptr<int> k){
    if(curr == nullptr)
        return;

    if((*k)> 0 && curr->right != nullptr && curr->right->height == 0){
        curr->right = nullptr;
        (*k)-=1;
    }
    if((*k) > 0 && curr->left != nullptr && curr->left->height == 0){
        curr->left = nullptr;
        (*k)-=1;
    }
    if(*k > 0)
        remove_while_half_full(curr->right, k);

    if((*k) > 0)
        remove_while_half_full(curr->left, k);
    if(curr->left == nullptr && curr->right == nullptr)
        curr->height = 0;
    else
        curr->height = curr->left == nullptr || curr->right == nullptr? 1: max(curr->left->height, curr->right->height)+1;
}


template<class T, class S>
AvlTree<T,S>::AvlTree(int number_of_nodes): size(number_of_nodes){
    int full_size = find_pow_of_2(number_of_nodes) - 1;
    this->root = AvlTree<T, S>::create_full_avl_tree(full_size);
    remove_while_half_full(this->root, std::make_shared<int>(full_size - number_of_nodes));
}

template<class T, class S>
StatusType AvlTree<T,S>::inner_put_inorder(std::shared_ptr<TreeNode<T,S>> nod, T key_arr[], S value_arr[], std::shared_ptr<int> min_index,int max_index){
    if(nod == nullptr)
        return FAILURE;
    this->inner_put_inorder(nod->left, key_arr, value_arr, min_index, max_index);
    if(*min_index <= max_index) {
        nod->key = key_arr[*min_index];
        nod->value = value_arr[*min_index];
        (*min_index)++;
    }
    this->inner_put_inorder(nod->right, key_arr, value_arr, min_index, max_index);
    return SUCCESS;
}

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::LL_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->left == nullptr)
        return nullptr;
    std::shared_ptr<TreeNode<T,S>> left = node_to_rotate->left;
    node_to_rotate->left = left->right;
    left->right = node_to_rotate;
    node_to_rotate->height = node_to_rotate->cal_height_non_recursive();
    left->height = left->cal_height_non_recursive();
    return left;
}

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::RR_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->right == nullptr)
        return nullptr;
    std::shared_ptr<TreeNode<T,S>> right = node_to_rotate->right;
    node_to_rotate->right = right->left;
    right->left = node_to_rotate;
    node_to_rotate->height = node_to_rotate->cal_height_non_recursive();
    right->height = right->cal_height_non_recursive();
    return right;
}

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::RL_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->right == nullptr||node_to_rotate->right->left == nullptr)
        return nullptr;
    node_to_rotate->right = LL_rotate(node_to_rotate->right);
    return RR_rotate(node_to_rotate);
}

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::LR_rotate(std::shared_ptr<TreeNode<T,S>> node_to_rotate){
    if(node_to_rotate == nullptr || node_to_rotate->left == nullptr||node_to_rotate->left->right == nullptr)
        return nullptr;
    node_to_rotate->left = RR_rotate(node_to_rotate->left);
    return LL_rotate(node_to_rotate);
}

template<class T, class S>
void AvlTree<T,S>::removeLeaf(std::shared_ptr<TreeNode<T,S>>* parent_ptr)
{
    (*parent_ptr) = nullptr;
    size--;
}

template<class T, class S>
void AvlTree<T,S>::removeNodeWithOnlyOneChild(std::shared_ptr<TreeNode<T,S>>* parent_ptr,
                                              std::shared_ptr<TreeNode<T,S>> current){
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


template<class T, class S>
void AvlTree<T,S>::removeNodeWithTwoChildren(std::shared_ptr<TreeNode<T,S>> node_to_remove){
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
    std::shared_ptr<TreeNode<T,S>> *replacement_node_parent_ptr;
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


template<class T, class S>
void AvlTree<T,S>::removeRoot() {
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

template<class T, class S>
std::shared_ptr<TreeNode<T,S>> AvlTree<T,S>::fixBalanceFactor(std::shared_ptr<TreeNode<T,S>> upper_node, T parent_key)
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


template<class T, class S>
StatusType AvlTree<T,S>::remove(T key){
    std::shared_ptr<TreeNode<T,S>> parent = find(key);
    std::shared_ptr<TreeNode<T,S>>* parent_ptr;
    std::shared_ptr<TreeNode<T,S>> current;

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

#endif //DS_WET_2_AVL_TREE_H