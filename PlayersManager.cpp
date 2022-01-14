#include "PlayersManager.h"
#include <assert.h>

int myHashFunction(int player_id){
    return player_id;
}


PlayersManager::PlayersManager(int k, int scale) :
    players_table(HashTable<int,Player,int (*)(int)>(&myHashFunction)),
    groups(UnionFind(k)){
    all_levels = std::make_shared<RankTree>();
    scores = new Score[scale+1];
    this->groups_count = k;
    this->scale = scale;
}


StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2){
    return groups.unite(groups.find(GroupID1), groups.find(GroupID2));

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
    return level >= 0? levels->get_rank(level) : 0;
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel,
                                                                int higherLevel, double *players) {
    if((GroupID < 0) || (GroupID > groups_count) || (players == nullptr)){
        return INVALID_INPUT;
    }
    if(score > scale || score <= 0){
        return FAILURE;
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
            getNumberOfPlayersWithScoreUntilLevel(levels, lowerLevel-1);
    if(total_number_of_players_between_levels == 0){
        return FAILURE;
    }

    number_of_players_between_level_with_score = getNumberOfPlayersWithScoreUntilLevel(score_levels, higherLevel) -
            getNumberOfPlayersWithScoreUntilLevel(score_levels, lowerLevel-1);
    *players = (double)(number_of_players_between_level_with_score) * 100 / (double)total_number_of_players_between_levels;

    return SUCCESS;
}


int space = 0;
void print_tree_aux(std::shared_ptr<RankTreeNode> node){
    if(node == nullptr)
        return;
    space++;
    print_tree_aux(node->left);
    for (int i = 0; i< space*10; i++)
        printf(" ");
    printf("key=%d value=%d sum=%d size=%d\n", node->key, node->value, node->sum_of_subtree, node->size_of_subtree);
    print_tree_aux(node->right);
    space--;

}

void print_tree(RankTree tree){
    print_tree_aux(tree.root);
}

StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level) {
    if(!m || GroupID < 0 || GroupID > groups_count || m <= 0)
        return INVALID_INPUT;
    std::shared_ptr<RankTree> levels;
    if (GroupID == 0)
        levels = all_levels;
    else{
        Group group = groups.get_group(groups.find(GroupID));
        levels = group.all_group_levels;
    }
    if (levels->get_size() - m < 0)
        return FAILURE;
    *level = (double) levels->sum_of_best_m(m) / (double)(m);
    return SUCCESS;
}

int min(int a, int b){
    return a<b? a : b;
}

StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers){
    if(!HigherBoundPlayers || !LowerBoundPlayers || m < 0 || GroupID < 0 || GroupID > groups_count || score <= 0 || score > scale)
        return INVALID_INPUT;
    std::shared_ptr<RankTree> levels;
    std::shared_ptr<RankTree> score_levels;
    if (GroupID == 0){
        levels = all_levels;
        score_levels = scores[score].score_levels;
    }
    else{
        Group group = groups.get_group(GroupID);
        levels = group.all_group_levels;
        score_levels = group.group_scores[score].score_levels;
    }
    if(m > levels->get_size())
        return FAILURE;
    std::shared_ptr<RankTreeNode> req_node = levels->find_nod_of_rank_m(levels->get_size() - m);
    //min_level = req_node->key
    assert(req_node);
    if(!req_node){
        *LowerBoundPlayers = *HigherBoundPlayers = 0;
        return SUCCESS;
    }
    int too_big = levels->get_size() - levels->get_rank(req_node->key);
    int good_or_too_big = levels->get_size() - levels->get_rank(req_node->key-1);
    int good = good_or_too_big - too_big;

    std::shared_ptr<RankTreeNode> nod = score_levels->get(req_node->key);
    if(!nod){
        *LowerBoundPlayers = *HigherBoundPlayers = 0;
        return SUCCESS;
    }
    int relevant_players = score_levels->get_rank(nod->key ) - score_levels->get_rank(nod->key-1);

    *HigherBoundPlayers = min(m-too_big, relevant_players);
    *LowerBoundPlayers = min(m-too_big-(good-relevant_players), relevant_players);
    if(*LowerBoundPlayers < 0)
        *LowerBoundPlayers = 0;
    return SUCCESS;
}