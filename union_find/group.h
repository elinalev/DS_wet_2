//
// Created by lior1 on 1/8/2022.
//
#include "score.h"
#ifndef MTM_2_PARTB_GROUP_H
#define MTM_2_PARTB_GROUP_H
#define MAX_SCORE 200

struct Group{
    Group();
    //we will think of group_scores[0] as all_group_levels
    Score group_scores[MAX_SCORE+1];
    std::shared_ptr<RankTree> all_group_levels;
    ///TODO: problems creating destructor, need to check if default one is good enough
    ~Group() = default;

    /// adding a group to a group
    /// \param smaller - the added group, should be smaller than current one for complexity
    /// \return SUCCESS or ALLOCATION_ERROR
    StatusType merge(const Group& smaller){
        try {
            *all_group_levels = RankTree::merge(*all_group_levels,*(smaller.all_group_levels));
            for(int i = 0; i <= MAX_SCORE; i++)
                group_scores[i].merge(smaller.group_scores[i]);
        }catch(const std::bad_alloc&){
            return ALLOCATION_ERROR;
        }
        return SUCCESS;
    }
    StatusType remove_group();
    int get_size(){
        if(group_scores[0].score_levels == nullptr)
            return -1;
        return group_scores[0].score_levels->get_size();
    }
};

#endif //MTM_2_PARTB_GROUP_H
