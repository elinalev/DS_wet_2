//
// Created by lior1 on 1/8/2022.
//
#include "union_find.h"

int main(){
    UnionFind u1(4);
    u1.print_union();
    u1.unite(u1.find(1), u1.find(2));
    u1.print_union();
    u1.unite(u1.find(1), 3);
    u1.print_union();
    Group g1 = u1.get_group(u1.find(1));

    int i = 1;
  return i;
}

