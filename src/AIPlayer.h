#ifndef DEF_AIPLAYER
#define DEF_AIPLAYER 

#include "board.h"
#include "player.h"

class AIPlayer: public Player {
    public:
        AIPlayer(Board* board): Player(false), board(board) {};
        ~AIPlayer() {};

        Move getMove();
        float negamax(int depth);
        float score();

        Board* board;

        float material_weight[7] = {0, 0, 5, 3, 3, 3, 1};
        int score_call;
};

#endif /* DEF_AIPLAYER */
