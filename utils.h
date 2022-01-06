<<<<<<< HEAD
#ifndef DS_WET_2_UTILS_H
#define DS_WET_2_UTILS_H

int max(int a, int b);
int find_pow_of_2(int n);

#endif //DS_WET_2_UTILS_H
=======
//
// Created by lior1 on 1/2/2022.
//

#ifndef DS_WET_2_UTILS_H
#define DS_WET_2_UTILS_H

int max (int x, int y) {
    return x>y? x : y;
}

int find_height(int n){
    if(n == 1)
        return 0;
    return 1 + find_height(n/2);
}

int find_pow_of_2(int n){
    int i = 1;
    for(; i  <=  n; i *=2){}
    return i;
}
#endif //DS_WET_2_UTILS_H
>>>>>>> 9abcfc61eadc7ae56016615447a0d1f7bdec63a3
