//
// Created by lior1 on 1/2/2022.
//

#ifndef DS_WET_2_UTILS_H
#define DS_WET_2_UTILS_H

int max (int x, int y) {
    return x>y? x : y;
}


int find_pow_of_2(int n){
    int i = 1;
    for(; i  <=  n; i *=2){}
    return i;
}
#endif //DS_WET_2_UTILS_H
