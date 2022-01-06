#ifndef DS_WET_2_HASH_TABLE_H
#define DS_WET_2_HASH_TABLE_H

#include <memory>
#include "avl_tree.h"

template <class Key, class Value, class HashFunc>
class HashTable {
    // the array will hold trees with the items
    static const int default_array_size = 10;
    std::shared_ptr<AvlTree<Key, std::shared_ptr<Value>>> hash_array[default_array_size];

    // the user will provide the hash function
    HashFunc hashFunction;

public:
    explicit HashTable(HashFunc hashFunction);

    // will do nothing if the key already exists
    void insert(Key key, Value value);

    // Returns nullptr if this key is not in the hashtable
    std::shared_ptr<Value> search(Key key);

    // will do nothing if this key doesn't exist
    void remove(Key key);
};

template <class Key, class Value, class HashFunc>
HashTable<Key,Value,HashFunc>::HashTable(HashFunc hashFunction){
    this->hashFunction = hashFunction;
    for (int i = 0; i < default_array_size; ++i) {
        this->hash_array[i] = std::make_shared<AvlTree<Key, std::shared_ptr<Value>>>();
    }
}


template <class Key, class Value, class HashFunc>
void HashTable<Key,Value,HashFunc>::insert(Key key, Value value){
    // find the index of this key
    int index = hashFunction(key);

    // TODO: check the load factor of this cell

    // get the tree for this index
    auto tree = hash_array[index];

    // check if this key is already in the tree
    if(tree->get(key)){
        return;
    }

    // insert the key
    tree->add(key, std::make_shared<Value>(value));
}


template <class Key, class Value, class HashFunc>
std::shared_ptr<Value> HashTable<Key,Value,HashFunc>::search(Key key) {
    // find the index of this key
    int index = hashFunction(key);

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
    int index = hashFunction(key);

    // get the tree for this index
    auto tree = hash_array[index];

    // delete the item
    tree->remove(key);

    // TODO: check the load factor of this cell
}

#endif //DS_WET_2_HASH_TABLE_H
