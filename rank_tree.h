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
    int number_of_nodes;

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
    StatusType inner_get_inorder(std::shared_ptr<RankTreeNode> node, int num_of_nodes, int  key_arr[],
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
    int inner_sum_of_best_m(std::shared_ptr<RankTreeNode> nod, int m);
    std::shared_ptr<RankTreeNode> inner_find_nod_of_rank_m(std::shared_ptr<RankTreeNode> nod, int m);

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
        return *zero + root->size_of_subtree;
    }

    //return the number of values smaller(not equal!) to a specific key
    int get_rank(int key);

    //return the sum of all values with smaller(not equal) keys
    int get_sum(int key);
    static RankTree merge(const RankTree& tree1, const RankTree& tree2);
    // constructors
    RankTree(): root(std::make_shared<RankTreeNode>(0)), zero(&(root->value)), size(0), number_of_nodes(1){};

    // basic tree operations
    StatusType add(int  key);
    StatusType add_0();
    std::shared_ptr<RankTreeNode> get(int  key); // returns null if the key isn't in the tree
    StatusType remove( int  key); // does nothing if the key doesn't exists in the tree


    // fill the arrays with correct keys and values
    StatusType get_inorder(int num_of_nodes, int  key_arr[], int val_arr[]) const;
    std::shared_ptr<RankTreeNode> get_node_of_rank_x(int x);
    int sum_of_best_m(int m);
    std::shared_ptr<RankTreeNode> find_nod_of_rank_m(int m);


    ///TODO: friends for testing, remove after
    friend int main();
    friend void print_tree(std::shared_ptr<RankTreeNode> node);
    friend void TEST_balanceFactor();
    };






#endif //DS_WET_1_AVL_TREE_H