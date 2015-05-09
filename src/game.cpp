#include <QDebug>
#include "game.h"

Game::Game() {
    board.loadDefaultPosition();

    pieces.load("pieces.png");
}

Game::~Game() {

}

Move* Game::inPossibleMove(int x, int y){
    foreach(Move m, possible_moves)
        if( m.to_x == x && m.to_y == y )
            return &m;
    return NULL;
}

void Game::mouseReleaseEvent( QMouseEvent* event ){
    int tx = event->x()/(height() / 8.0f);
    int ty = event->y()/(height() / 8.0f);
    if( inBounds(tx, ty) ){
        if( selection_mode && selection_x == tx && selection_y == ty ){
            selection_mode = false;
        } else if( !selection_mode ) {
            possible_moves = board.possibleMoves(tx, ty);
            if( possible_moves.size() != 0 ){
                selection_mode = true;
                selection_x = tx;
                selection_y = ty;
            }
        } else if( selection_mode ) {
            Move* move = inPossibleMove(tx, ty);
            if( move != NULL ){
                board.save();
                board.applyMove(*move);
                selection_mode = false;
            }
        }
    } else {
//        board.load();
    }
    repaint();
}

void Game::paintEvent(QPaintEvent* pe){
    float tile_size = height() / 8.0f;

    QPainter painter(this);
    for( int i=0 ; i<8 ; i++ )
        for( int j=0 ; j<8 ; j++ ) {

            // tile color
            if( selection_mode && selection_x == i && selection_y == j )
                painter.setBrush( QColor("pink") );
            else if( selection_mode && inPossibleMove(i, j) != NULL )
                painter.setBrush( QColor("yellow") );
            else
                painter.setBrush( (i+j)%2 ? QColor("grey") : QColor("white") );
//            if( board.state.enpassant && board.state.enpassant_x == i && board.state.enpassant_y == j ) painter.setBrush(QColor("blue"));
            painter.drawRect( i*tile_size, j*tile_size, tile_size, tile_size);

            // piece
            char tile = board.state.board[i][j];
            if( TYPE(tile) != T_EMPTY )
                painter.drawPixmap( i*tile_size, j*tile_size, tile_size, tile_size, pieces, 213*(TYPE(tile)-1), COLOR(tile) == C_WHITE ? 0 : 213, 213, 213);
        }


    QString status = "Status: ";
    if( board.state.status == S_NORMAL )
        status += QString(board.state.white_to_play ? "White" : "Black") + " to play";
    else
        status += (board.state.status == S_MATE ? "Mate" : "Slatemate");

    painter.setBrush( QColor("green") );
    painter.drawText(height() + 20, 20, status);
}
