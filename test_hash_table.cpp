#include "hash_table.h"
#include <iostream>
#include <cassert>

int myHashFunction(int id){
    return id;
}

void TEST_CreateHashTable(){
    auto ht = HashTable<int,int,int (*)(int)>(&myHashFunction);

    assert(ht.search(1) == nullptr);
    ht.insert(1,1);
    assert(ht.search(1) != nullptr);
    ht.remove(1);
    assert(ht.search(1) == nullptr);
}

void TEST_resizeHashTable(){
    auto ht = HashTable<int,int,int (*)(int)>(&myHashFunction);

    for (int i = 0; i < 121; ++i) {
        ht.insert(i,i);
    }
    for (int i = 0; i < 3; ++i) {
        assert(ht.search(i) != nullptr);
    }
}

void TEST_CreateCrashHashTable(){
    auto ht = HashTable<int,int,int (*)(int)>(&myHashFunction);

    assert(ht.search(1) == nullptr);
    ht.insert(1,1);
    ht.insert(8,8);
    ht.insert(15,15);
    assert(ht.search(1) != nullptr);
    ht.remove(1);
    assert(ht.search(1) == nullptr);
}

int main(){
    std::cout << "Running TEST_CreateHashTable" <<std::endl;
    TEST_CreateHashTable();
    std::cout << "Running TEST_CreateCrashHashTable" <<std::endl;
    TEST_CreateCrashHashTable();
    std::cout << "Running TEST_resizeHashTable" <<std::endl;
    TEST_resizeHashTable();

    return 0;
}