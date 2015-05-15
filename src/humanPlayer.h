#ifndef DEF_HUMANPLAYER
#define DEF_HUMANPLAYER 

#include "board.h"
#include "player.h"

class HumanPlayer: public Player {
    public:
        HumanPlayer(): Player(true) {};
        ~HumanPlayer() {};

        Move getMove();
        void inputMove(Move m);

        bool move_available = false;
        Move move_to_send;
};

#endif /* DEF_HUMANPLAYER */
