//
// Created by lior1 on 1/10/2022.
//
#include "PlayersManager.h"

int main(){
    PlayersManager manager(10, 10);
    manager.AddPlayer(1, 1, 2);
    manager.IncreasePlayerIDLevel(1,8);
    manager.AddPlayer(2, 1, 2);
    manager.AddPlayer(3, 1, 3);
    manager.AddPlayer(4, 1, 4);
    manager.AddPlayer(5, 1, 4);
    manager.IncreasePlayerIDLevel(2,6);
    manager.IncreasePlayerIDLevel(3,6);
    manager.IncreasePlayerIDLevel(4,6);
    manager.IncreasePlayerIDLevel(5,6);
    int min, max;
    if(manager.GetPlayersBound(1,4,3, &min, &max) == SUCCESS)
        printf("\n%d, %d\n\n", min, max);
    double d;
    printf("%d\n", manager.AverageHighestPlayerLevelByGroup(1,3, &d));
    printf("%f",d);

    return 0;
}