//
// Created by lior1 on 1/8/2022.
//
#include "rank_tree.h"

RankTreeNode::RankTreeNode(int key) : key(key), value(0), right(nullptr) , left(nullptr), height(0), size_of_subtree(0), sum_of_subtree(0){}

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
    return key < 0? 0 : *zero + inner_get_rank(key, root);
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
    if (number_of_nodes == 0 && node_to_put != nullptr && upper_node == nullptr) {
        return node_to_put;
    }

    if (upper_node == nullptr || node_to_put == nullptr)
        return nullptr;

    // save the node in the right place (no pun intended)
    if (*upper_node < *node_to_put) {
        upper_node->right = upper_node->right ? inner_add(upper_node->right, node_to_put) : node_to_put;
        upper_node->right->update_details();
    } else if (*upper_node == *node_to_put) {
        upper_node->value++;
        upper_node->update_details();
    } else{
        upper_node->left = upper_node->left ? inner_add(upper_node->left, node_to_put) : node_to_put;
        upper_node->update_details();
    }

    // check the balance factor and do a roll if necessary
    if (upper_node->get_balance_factor() == 2)
        return (upper_node->left->get_balance_factor() >= 0 ? LL_rotate(upper_node) : LR_rotate(upper_node))->update_details();
    else if (upper_node->get_balance_factor() == -2)
        return (upper_node->right->get_balance_factor() <= 0 ? RR_rotate(upper_node) : RL_rotate(upper_node))->update_details();
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
    int size1 = tree1.number_of_nodes, size2 = tree2.number_of_nodes, index = 0;
    //we need to copy 0 even if its node does not represent any value
    //therefore not counted in size
    int val_arr1[size1], key_arr1[size1], val_arr2[size2], key_arr2[size2], key_arr[size1+size2], val_arr[size1+size2];
    tree1.get_inorder(size1, key_arr1, val_arr1);
    tree2.get_inorder(size2, key_arr2, val_arr2);
    int size = merge_arr(key_arr, key_arr1, key_arr2, val_arr, val_arr1, val_arr2, size1, size2);
    RankTree tree;
    tree.root = create_half_full_avl_tree(find_pow_of_2(size)-1, size, key_arr, val_arr, &index);
    tree.number_of_nodes = size;
    tree.zero = &(tree.get(0))->value;
    return tree;
}
std::shared_ptr<RankTreeNode> RankTree::find_nod_of_rank_m(int m){
    if(m < 0 || m > get_size())
        return nullptr;
    return m - * zero < 0 ? get(0) : inner_find_nod_of_rank_m(root, m - *zero);
}
std::shared_ptr<RankTreeNode> RankTree::inner_find_nod_of_rank_m(std::shared_ptr<RankTreeNode> nod, int m){
    if(!nod)
        return nullptr;
    if(!nod->left){
        if(nod->value < m || nod->value == 0)
            return inner_find_nod_of_rank_m(nod->right, m - nod->value);
        return nod;
    }
    int right_and_this = m - nod->left->size_of_subtree;
    int right = right_and_this - nod->value;
    if(right <= 0)
        return nod;
    return inner_find_nod_of_rank_m(nod->right, right);
}

int RankTree::sum_of_best_m(int m) {
    if(m > root->size_of_subtree + *zero || m < 0)
        return -1;
    return inner_sum_of_best_m(root, m);
}

int RankTree::inner_sum_of_best_m(std::shared_ptr<RankTreeNode> nod, int m){
    if(!nod)
        return 0;
    int num_from_left_and_this = m;
    if(nod->right)
        num_from_left_and_this -= nod->right->size_of_subtree;
    if(num_from_left_and_this <= 0)
        return inner_sum_of_best_m((nod->right), m);
    int num_from_this = nod->value > num_from_left_and_this?
            num_from_left_and_this : nod->value;
    int num_from_left = num_from_left_and_this - num_from_this;
    return nod->key * num_from_this +
            (nod->right? nod->right->sum_of_subtree : 0) +
            inner_sum_of_best_m(nod->left, num_from_left);
}


StatusType RankTree::add_0(){
    if(zero){
        (*zero)++;
    } else
        return FAILURE;
    return SUCCESS;
}


StatusType RankTree::add(int key){
    if(key == 0){
        return add_0();
    }
    try {
        if(!get(key))
            number_of_nodes++;
        std::shared_ptr<RankTreeNode> new_node = std::make_shared<RankTreeNode>(key);
        new_node->value = MIN_COUNT;
        new_node->update_details();
        this->root = RankTree::inner_add(root, new_node);
    }catch(const std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
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
                                       int num_of_nodes, int  key_arr[], int val_arr[], int *index) const{

    if(node == nullptr)
        return SUCCESS;

    if(inner_get_inorder(node->left, num_of_nodes, key_arr, val_arr, index) == FAILURE)
        return FAILURE;

    if(num_of_nodes == *index)
        // using the FAILURE flag to stop
        return FAILURE;

    // fill array
    key_arr[*index] = node->key;
    val_arr[*index] = node->value;
    (*index)++;

    if(inner_get_inorder(node->right, num_of_nodes, key_arr, val_arr, index) == FAILURE)
        return FAILURE;

    return SUCCESS;
}


StatusType RankTree::get_inorder(int num_of_nodes, int  key_arr[], int val_arr[]) const{
    int index = 0;
    return inner_get_inorder(this->root, num_of_nodes, key_arr, val_arr, &index);
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
    number_of_nodes--;
}


void RankTree::removeNodeWithOnlyOneChild(std::shared_ptr<RankTreeNode>* parent_ptr,
                                          std::shared_ptr<RankTreeNode> current){
    // left child exists
    if(!(current->right) && current->left)
    {
        (*parent_ptr) = current->left;
        number_of_nodes--;
        return;
    }
    // right child exists
    if(!(current->left) && current->right)
    {
        (*parent_ptr) = current->right;
        number_of_nodes--;
        return;
    }
}



void RankTree::removeNodeWithTwoChildren(std::shared_ptr<RankTreeNode> node_to_remove){
    // step 1: find the replacement node (go right and then all the way left)
    auto replacement_node = node_to_remove->right;
    auto replacement_node_parent = node_to_remove;

    auto fix = node_to_remove->right;
    auto fix_parent = node_to_remove;

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

    //fixing nodes height
    while(fix){
        fix->update_details();
        fix_parent->update_details();
        fix_parent = fix;
        fix = fix->left;
    }

}



void RankTree::removeRoot() {
    if(!(root->right) && !(root->left)){
        root = nullptr;
        number_of_nodes--;
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
        number_of_nodes--;
        root->update_details();
    }
}


std::shared_ptr<RankTreeNode> RankTree::fixBalanceFactor(std::shared_ptr<RankTreeNode> upper_node, int parent_key)
{
    // recurse until we find the parent
    if (upper_node.get()->key < parent_key)
    {
        upper_node->right = fixBalanceFactor(upper_node->right, parent_key);
        upper_node->right->update_details();
    }
    else if (upper_node.get()->key > parent_key)
    {
        upper_node->left = fixBalanceFactor(upper_node->left, parent_key);
        upper_node->left->update_details();
    }
    else{
        upper_node->update_details();
        return upper_node;
    }
    upper_node->update_details();
    if (upper_node->get_balance_factor() == 2)
    {
        return upper_node->left->get_balance_factor() >= 0 ? LL_rotate(upper_node) : LR_rotate(upper_node);
    }
    else if (upper_node->get_balance_factor() == -2)
    {
        return upper_node->right->get_balance_factor() <= 0 ? RR_rotate(upper_node) : RL_rotate(upper_node);
    }

    // update the node height
    upper_node->update_details();

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
        root->update_details();
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
    current->update_details();
    parent->update_details();
    root = fixBalanceFactor(root, parent.get()->key);
    root->update_details();
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
