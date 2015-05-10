#ifndef DEF_PLAYER
#define DEF_PLAYER 

#include "board.h"

class Player {
    public:
        Player() {};
        ~Player() {};

        virtual Move getMove();
};

#endif /* DEF_PLAYER */
