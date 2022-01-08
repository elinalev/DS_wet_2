//
// Created by lior1 on 1/8/2022.
//

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