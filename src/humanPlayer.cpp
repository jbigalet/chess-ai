#include <QCoreApplication>
#include "humanPlayer.h"

Move HumanPlayer::getMove(){
    move_available = false;
    while( !move_available )
        QCoreApplication::processEvents();
    return move_to_send;
}

void HumanPlayer::inputMove(Move m){
    move_to_send = m;
    move_available = true;
}
