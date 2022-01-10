//
// Created by lior1 on 1/10/2022.
//
#include "PlayersManager.h"

int main(){
    PlayersManager manager(10, 10);
    manager.AddPlayer(1, 1, 1);
    manager.AddPlayer(2, 1, 2);
    manager.AddPlayer(2, 1, 3);
    manager.AddPlayer(2, 1, 4);
    manager.IncreasePlayerIDLevel(1,4);
    double d;
    printf("%d\n", manager.AverageHighestPlayerLevelByGroup(1,3, &d));
    printf("%f",d);
    return 0;
}