#ifndef DEF_AIPLAYER
#define DEF_AIPLAYER 

#include "board.h"
#include "player.h"

class AIPlayer: public Player {
    public:
        AIPlayer(): Player(false) {};
        ~AIPlayer() {};

        Move getMove();
};

#endif /* DEF_AIPLAYER */
