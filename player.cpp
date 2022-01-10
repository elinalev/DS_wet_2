#include "player.h"

Player::Player(int player_id, int group_id, int score):
    player_id(player_id), group_id(group_id), score(score){
    level = 0;
}

int Player::getID() {
    return player_id;
}

int Player::getGroupId(){
    return group_id;
}

int Player::getLevel(){
    return level;
}

int Player::getScore(){
    return score;
}

void Player::setScore(int new_score){
    this->score = new_score;
}

void Player::setLevel(int new_level){
    this->level = new_level;
}