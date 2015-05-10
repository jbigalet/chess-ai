#ifndef DEF_HUMANPLAYER
#define DEF_HUMANPLAYER 

#include "board.h"
#include "player.h"

class HumanPlayer: public Player {
    public:
        HumanPlayer() {};
        ~HumanPlayer() {};

        Move getMove();
};

#endif /* DEF_HUMANPLAYER */
