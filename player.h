#ifndef MTM_2_PARTB_PLAYER_H
#define MTM_2_PARTB_PLAYER_H

class Player{
    int player_id;
    int group_id;
    int score;
    int level;
public:
    Player(int player_id, int group_id, int score);
    int getID();
    int getGroupId();
    int getScore();
    int getLevel();
};
#endif //MTM_2_PARTB_PLAYER_H
