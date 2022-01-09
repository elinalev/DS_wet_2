#include "player.h"

Player::Player(int player_id, int group_id, int score):
    player_id(player_id), group_id(group_id), score(score){

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