#include "group.h"

Group::Group(){
    all_group_levels = std::make_shared<RankTree>();
}

StatusType Group::remove_group(){
    for(int i = 0; i < MAX_SCORE+1; i++){
        group_scores[i].score_levels = nullptr;
    }
    return SUCCESS;
}
