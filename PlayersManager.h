#ifndef DS_WET_2_PLAYERSMANAGER_H
#define DS_WET_2_PLAYERSMANAGER_H

#include "library2.h"
#include "hash_table.h"
#include "player.h"
#include "rank_tree.h"
#include "union_find/union_find.h"

class PlayersManager{
    // save all the players
    HashTable<int,Player,int (*)(int)> players_table;

    // save all groups
    UnionFind groups;
    int groups_count;

    // RankTree of levels for each score
    Score *scores;
    int scale;

    // RankTree of level
    RankTree all_levels;

public:
    explicit PlayersManager(int k, int scale);
    StatusType MergeGroups(int GroupID1, int GroupID2);
    StatusType AddPlayer(int PlayerID, int GroupID, int score);
    StatusType RemovePlayer(int PlayerID);
    StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
    StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double * players);
    StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
    StatusType GetPlayersBound(int GroupID, int score, int m,
                               int * LowerBoundPlayers, int * HigherBoundPlayers);
};

#endif //DS_WET_2_PLAYERSMANAGER_H
