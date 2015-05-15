#ifndef DEF_AIPLAYER
#define DEF_AIPLAYER 

#include "board.h"
#include "player.h"

class AIPlayer: public Player {
    public:
        AIPlayer(Board* board): Player(false), board(board) {};
        ~AIPlayer() {};

        Move getMove();

        Board* board;
};

#endif /* DEF_AIPLAYER */
