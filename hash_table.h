#ifndef DS_WET_2_HASH_TABLE_H
#define DS_WET_2_HASH_TABLE_H

#include <memory>
#include "avl_tree.h"

template <class Key, class Value, class HashFunc>
class HashTable {
    // the array will hold trees with the items
    static const int default_array_size = 10;
    static const int load_factor = 3;
    static const int min_factor = 4;
    std::shared_ptr<AvlTree<Key, std::shared_ptr<Value>>> *hash_array;

    // the user will provide the hash function
    HashFunc hashFunction;

    int current_size = default_array_size;
    int items_count;

    // inner functions
    void inner_insert(Key key, Value value);
    void resize_hash_table(bool redouble);

public:
    explicit HashTable(HashFunc hashFunction);

    // will do nothing if the key already exists
    void insert(Key key, Value value);

    // Returns nullptr if this key is not in the hashtable
    std::shared_ptr<Value> search(Key key);

    // will do nothing if this key doesn't exist
    void remove(Key key);

    ~HashTable();
};

/* inner function*/
template <class Key, class Value, class HashFunc>
void HashTable<Key,Value,HashFunc>::resize_hash_table(bool redouble){
    items_count = 0;
    int old_size = this->current_size;
    this->current_size = redouble ? this->current_size*2 : this->current_size/2 ;

    auto old_hash_array = hash_array;
    hash_array = new std::shared_ptr<AvlTree<Key, std::shared_ptr<Value>>>[this->current_size];
    for (int i = 0; i < this->current_size; ++i) {
        hash_array[i] = std::make_shared<AvlTree<Key, std::shared_ptr<Value>>>();
    }

    // hash all items to the new array
    for (int i = 0; i < old_size; ++i) {
        auto tree = old_hash_array[i];
        int tree_size = tree->get_size();
        Key key_arr[tree_size];
        std::shared_ptr<Value> val_arr[tree_size];
        tree->get_inorder(tree->get_size(),key_arr, val_arr);
        for (int j = 0; j < tree_size; ++j) {
            inner_insert(key_arr[j],*val_arr[j]);
        }
    }

    delete[] old_hash_array;
}


/* user functions */
template <class Key, class Value, class HashFunc>
HashTable<Key,Value,HashFunc>::HashTable(HashFunc hashFunction){
    items_count = 0;
    this->hashFunction = hashFunction;
    this->hash_array = new std::shared_ptr<AvlTree<Key, std::shared_ptr<Value>>>[default_array_size];
    for (int i = 0; i < this->current_size; ++i) {
        this->hash_array[i] = std::make_shared<AvlTree<Key, std::shared_ptr<Value>>>();
    }
}


template <class Key, class Value, class HashFunc>
void HashTable<Key,Value,HashFunc>::inner_insert(Key key, Value value){
    // find the index of this key
    int index = (hashFunction(key)) % (this->current_size);

    // get the tree for this index
    auto tree = hash_array[index];

    // check if this key is already in the tree
    if(tree->get(key)){
        return;
    }

    // insert the key
    tree->add(key, std::make_shared<Value>(value));
    items_count++;
}

template <class Key, class Value, class HashFunc>
void HashTable<Key,Value,HashFunc>::insert(Key key, Value value){
    inner_insert(key, value);

    if(items_count > current_size * load_factor){
        resize_hash_table(true);
    }
}

template <class Key, class Value, class HashFunc>
std::shared_ptr<Value> HashTable<Key,Value,HashFunc>::search(Key key) {
    // find the index of this key
    int index = (hashFunction(key)) % current_size;

    // get the tree for this index
    auto tree = hash_array[index];

    auto item = tree->get(key);
    if(item){
        return item->value;
    }

    return nullptr;
}


template <class Key, class Value, class HashFunc>
void HashTable<Key,Value,HashFunc>::remove(Key key) {
    // find the index of this key
    int index = (hashFunction(key)) % current_size;

    // get the tree for this index
    auto tree = hash_array[index];

    // delete the item
    tree->remove(key);
    items_count--;

    if(items_count < (current_size * load_factor)/min_factor){
        resize_hash_table(false);
    }
}


template <class Key, class Value, class HashFunc>
HashTable<Key,Value,HashFunc>::~HashTable<Key, Value, HashFunc>() {
    delete[] this->hash_array;
}
#endif //DS_WET_2_HASH_TABLE_H
