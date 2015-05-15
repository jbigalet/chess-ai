#include <QDebug>
#include "AIPlayer.h"

Move AIPlayer::getMove(){
    // debug: static move
//    return Move(0, 1, 0, 2);

    // debug: random move
//    QVector<Move> moves = board->allPossibleMoves(true);
//    return moves.at(qrand()%moves.size());

    // debug: keep track of the number of score call
    score_call = 0;

    // minmax
    float max = -1000000.0f; // ~ -infinity
    Move best_move;
    foreach(Move move, board->allPossibleMoves(true)){
        board->save();
        board->applyMove(move);
        float score = -negamax(2);
        board->load();

        if( score > max ){
            max = score;
            best_move = move;
        }
    }

    qDebug() << "score call: " << score_call;
    return best_move;
}

float AIPlayer::negamax(int depth){
    if( depth == 0 )
        return score();

    float max = -1000000.0f; // ~ -infinity
    foreach(Move move, board->allPossibleMoves(true)){
        board->save();
        board->applyMove(move);
        float score = -negamax(depth-1);
        board->load();

        if( score > max )
            max = score;
    }

    return max;
}

float AIPlayer::score(){
    score_call++;

    // game ended
    if( board->state.status == S_MATE )
        return 1000000.0f; // ~ infinity;
    if( board->state.status == S_STALEMATE )
        return 0.0f;

    // material difference
    float material_diff = 0;
    for(int i=0 ; i<8 ; i++)
        for(int j=0 ; j<8 ; j++){
            char tile = board->state.board[i][j];
            material_diff += material_weight[TYPE(tile)] * (COLOR(tile) ? -1 : 1);
        }

    if( !board->state.white_to_play )
        material_diff = -material_diff;

    return material_diff;
}
