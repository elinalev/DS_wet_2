//
// Created by lior1 on 1/8/2022.
//
#include "../rank_tree.h"
#ifndef MTM_2_PARTB_SCORE_H
#define MTM_2_PARTB_SCORE_H

struct Score{
    std::shared_ptr<RankTree> score_levels;
    Score(){
        score_levels = std::make_shared<RankTree>();
    }
    ~Score() = default;
    /// merge 2 scores, might raise an allocation error
    /// \param other score to add to current score
    void merge(const Score& other){
        *score_levels = RankTree::merge(*score_levels, *other.score_levels);
    }
};
#endif //MTM_2_PARTB_SCORE_H
