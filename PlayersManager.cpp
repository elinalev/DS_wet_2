#include "PlayersManager.h"


int myHashFunction(int player_id){
    return player_id;
}


PlayersManager::PlayersManager(int k, int scale) :
    players_table(HashTable<int,Player,int (*)(int)>(&myHashFunction)),
    groups(UnionFind(k)){
    all_levels = std::make_shared<RankTree>();
    scores = new Score[scale];
    this->groups_count = k;
    this->scale = scale;
}


StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2){
    //TODO: implement
    return FAILURE;
}


StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score){
    if((GroupID <= 0) || (GroupID > groups_count) || (score <= 0) || (score > scale) || (PlayerID <= 0)){
        return INVALID_INPUT;
    }

    if(players_table.search(PlayerID) != nullptr){
        return FAILURE;
    }

    // update players, groups, scores, all_levels
    Player player = Player(PlayerID, GroupID, score);
    players_table.insert(PlayerID, player);

    auto group = groups.get_group(GroupID);

    // update all level trees - will add this in O(1)
    group.all_group_levels->add(0);
    group.group_scores[score].score_levels->add(0);
    scores[score].score_levels->add(0);
    all_levels->add(0);

    return SUCCESS;
}


StatusType PlayersManager::RemovePlayer(int PlayerID){
    if(PlayerID <= 0){
        return INVALID_INPUT;
    }

    auto player = players_table.search(PlayerID);
    if(player == nullptr){
        return FAILURE;
    }

    int player_level = player->getLevel();
    int player_score = player->getScore();

    auto group = groups.get_group(player->getGroupId());
    group.all_group_levels->remove(player_level);
    group.group_scores[player_score].score_levels->remove(player_level);
    scores[player_score].score_levels->remove(player_level);
    all_levels->remove(player_level);

    players_table.remove(PlayerID);

    return SUCCESS;
}


StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease){
    if((PlayerID <= 0) || (LevelIncrease <= 0)){
        return INVALID_INPUT;
    }

    auto player = players_table.search(PlayerID);
    if(player == nullptr){
        return FAILURE;
    }

    int player_level = player->getLevel();
    int new_player_level = player_level + LevelIncrease;
    int player_score = player->getScore();
    auto group = groups.get_group(player->getGroupId());

    group.all_group_levels->remove(player_level);
    group.all_group_levels->add(new_player_level);

    group.group_scores[player_score].score_levels->remove(player_level);
    group.group_scores[player_score].score_levels->add(new_player_level);

    scores[player_score].score_levels->remove(player_level);
    scores[player_score].score_levels->add(new_player_level);

    all_levels->remove(player_level);
    all_levels->add(new_player_level);

    player->setLevel(new_player_level);

    return SUCCESS;
}


StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore){
    if((PlayerID <= 0) || (NewScore <= 0) || (NewScore > scale)){
        return INVALID_INPUT;
    }

    auto player = players_table.search(PlayerID);
    if(player == nullptr){
        return FAILURE;
    }

    int player_level = player->getLevel();
    int player_score = player->getScore();
    auto group = groups.get_group(player->getGroupId());

    group.group_scores[player_score].score_levels->remove(player_level);
    group.group_scores[NewScore].score_levels->add(player_level);

    scores[player_score].score_levels->remove(player_level);
    scores[NewScore].score_levels->add(player_level);

    player->setScore(NewScore);

    return SUCCESS;

}


int getNumberOfPlayersWithScoreUntilLevel(std::shared_ptr<RankTree> &levels, int level){
    return levels->get_rank(level);
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel,
                                                                int higherLevel, double *players) {
    if((GroupID < 0) || (GroupID > groups_count) || (players == nullptr)){
        return INVALID_INPUT;
    }

    std::shared_ptr<RankTree> score_levels;
    std::shared_ptr<RankTree> levels;

    int total_number_of_players_between_levels;
    int number_of_players_between_level_with_score;

    if (GroupID == 0){
        score_levels = scores[score].score_levels;
        levels = all_levels;
    }
    else{
        auto group = groups.get_group(GroupID);
        score_levels = group.group_scores[score].score_levels;
        levels = group.all_group_levels;
    }

    total_number_of_players_between_levels = getNumberOfPlayersWithScoreUntilLevel(levels, higherLevel) -
            getNumberOfPlayersWithScoreUntilLevel(levels, lowerLevel);

    number_of_players_between_level_with_score = getNumberOfPlayersWithScoreUntilLevel(score_levels, higherLevel) -
            getNumberOfPlayersWithScoreUntilLevel(score_levels, lowerLevel);

    *players = (double(number_of_players_between_level_with_score) * 100 / total_number_of_players_between_levels);

    return SUCCESS;
}


StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level) {
    //TODO: implement
    return FAILURE;
}


StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers){
    //TODO: implement
    return FAILURE;
}