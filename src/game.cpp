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
        } else if( !selection_mode && TYPE(board.board[tx][ty]) != T_EMPTY ) {
            selection_mode = true;
            selection_x = tx;
            selection_y = ty;
            possible_moves = board.possibleMoves(tx, ty);
        } else if( selection_mode ) {
            Move* move = inPossibleMove(tx, ty);
            if( move != NULL ){
                board.applyMove(*move);
                selection_mode = false;
            }
        }
        repaint();
    }
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
            painter.drawRect( i*tile_size, j*tile_size, tile_size, tile_size);

            // piece
            char tile = board.board[i][j];
            if( TYPE(tile) != T_EMPTY )
                painter.drawPixmap( i*tile_size, j*tile_size, tile_size, tile_size, pieces, 213*(TYPE(tile)-1), COLOR(tile) == C_WHITE ? 0 : 213, 213, 213);
        }
}
