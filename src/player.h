#ifndef DEF_PLAYER
#define DEF_PLAYER 

#include "board.h"

class Player {
    public:
        Player() {};
        Player(bool ni): needs_input(ni) {};
        ~Player() {};

        virtual Move getMove();
        virtual void inputMove(Move m) {};

        bool needs_input;
};

#endif /* DEF_PLAYER */
