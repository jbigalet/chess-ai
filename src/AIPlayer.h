#ifndef DEF_AIPLAYER
#define DEF_AIPLAYER 

#include "board.h"
#include "player.h"

class AIPlayer {
    public:
        AIPlayer() {};
        ~AIPlayer() {};

        Move getMove();
};

#endif /* DEF_AIPLAYER */
