#include "utils.h"

int max(int a, int b){
    return a<b? b : a;
}

int find_pow_of_2(int n){
    int i = 1;
    for(; i  <=  n; i *=2){}
    return i;
}