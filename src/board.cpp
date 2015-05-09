#include <QDebug>
#include "board.h"

Board::Board() {
    memset(board, 0, 64);
}

void Board::loadDefaultPosition() {
    for( int color=0 ; color<2 ; color++ ){
        int r = color*7;
        char c = (color ? C_WHITE : C_BLACK);

        // pawns
        for( int i=0 ; i<8 ; i++ )
            board[i][1+color*5] = T_PAWN | c;

        board[0][r] = board[7][r] = T_ROOK | c;
        board[1][r] = board[6][r] = T_KNIGHT | c;
        board[2][r] = board[5][r] = T_BISHOP | c;
        board[3][r] = T_KING | c;
        board[4][r] = T_QUEEN | c;
    }
}

QVector<Move> Board::possibleMoves(int x, int y){
    QVector<Move> moves;

    char color = COLOR(board[x][y]);

    switch(TYPE(board[x][y])) {
        case T_KING:
            for(int i=-1 ; i<=1 ; i++)
                for(int j=-1 ; j<=1 ; j++)
                    if( (i!=0 || j!=0 ) && canGo(x+i, y+j, color) ){
                        Move m = { x, y, x+i, y+j };
                        moves.append(m);
                    }
            break;

        case T_QUEEN:
            break;

        case T_BISHOP:
            break;

        case T_KNIGHT:
            for(int i=1 ; i<=2 ; i++)
                for(int i_sign=-1 ; i_sign<=2 ; i_sign+=2)
                    for(int j_sign=-1 ; j_sign<=1 ; j_sign+=2){
                        int a = x+i*i_sign;
                        int b = y+(3-i)*j_sign;
                        if( canGo(a, b, color) ){
                            Move m = { x, y, a, b };
                            moves.append(m);
                        }
                    }
            break;

        case T_ROOK:
            break;

        case T_PAWN:
            int orientation = color ? 1 : -1;

            // straight
            if( TYPE(board[x][y+orientation]) == T_EMPTY ){
                Move m = { x, y, x, y+orientation };
                moves.append(m);

                // 2nd row: can move 2
                if( (y-orientation)%7 == 0  && TYPE(board[x][y+2*orientation]) == T_EMPTY ){
                    Move m2 = { x, y, x, y+2*orientation };
                    moves.append(m2);
                }
            }

            // diag eat @TODO prise en passant
            for(int sign=-1 ; sign<=1 ; sign+=2)
                if( inBounds(x+sign, y+orientation) ){
                    char tile = board[x+sign][y+orientation];
                    if( TYPE(tile) != T_EMPTY && COLOR(tile) != color ){
                        Move m3 = { x, y, x+sign, y+orientation };
                        moves.append(m3);
                    }
                }
            break;
    }

    return moves;
}

bool Board::canGo(int x, int y, char color){
    return inBounds(x, y) && isEatable(board[x][y], color);
}

void Board::applyMove(Move move){
    // we assume its legal
    board[move.to_x][move.to_y] = board[move.from_x][move.from_y];
    board[move.from_x][move.from_y] = T_EMPTY;
}
