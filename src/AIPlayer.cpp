#include "AIPlayer.h"

Move AIPlayer::getMove(){
    // debug: static move
//    return Move(0, 1, 0, 2);

    // debug: random move
    QVector<Move> moves = board->allPossibleMoves(true);
    return moves.at(qrand()%moves.size());
}
