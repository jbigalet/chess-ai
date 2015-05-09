#include <QDebug>
#include "board.h"

Board::Board() {
    memset(board, 0, 64);
    memset(can_castle, 1, 4);
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
        board[3][r] = T_QUEEN | c;
        board[4][r] = T_KING | c;
    }
}

QVector<Move> Board::possibleMoves(int x, int y){
    QVector<Move> moves;

    char color = COLOR(board[x][y]);
    bool queen = false; // to easily handle queen = bishop + rook

    switch(TYPE(board[x][y])) {
        case T_KING:
            for(int i=-1 ; i<=1 ; i++)
                for(int j=-1 ; j<=1 ; j++)
                    if( (i!=0 || j!=0 ) && canGo(x+i, y+j, color) ){
                        Move m = { x, y, x+i, y+j };
                        moves.append(m);
                    }

            // castling
            for(int i=0 ; i<=1 ; i++) // left then right
                if( can_castle[color == C_WHITE ? 0 : 1][i] ){
                    bool open = true;
                    for(int j=1+5*i ; j!=4 ; j+=1-2*i)
                        if( board[j][y] != T_EMPTY ){
                            open = false;
                            break;
                        }
                    if( open ){
                        Move m = { x, y, 2+4*i, y };
                        moves.append(m);
                    }
                }
            break;

        case T_QUEEN:
            queen = true;

        case T_BISHOP:
            for(int sx=-1 ; sx<=1 ; sx+=2)
                for(int sy=-1 ; sy<=1 ; sy+=2){
                    int a=x+sx;
                    int b=y+sy;
                    while( canGo(a, b, color) ){
                        Move m = { x, y, a, b };
                        moves.append(m);
                        if( TYPE(board[a][b]) != T_EMPTY )
                            break;
                        a += sx;
                        b += sy;
                    }
                }
            if( !queen )
                break;

        case T_ROOK:
            for(int sign=-1 ; sign<=1 ; sign+=2){
                // rows
                int a=x+sign;
                while( canGo(a, y, color) ){
                    Move m = { x, y, a, y };
                    moves.append(m);
                    if( TYPE(board[a][y]) != T_EMPTY )
                        break;
                    a += sign;
                }

                // columns
                int b=y+sign;
                while( canGo(x, b, color) ){
                    Move m = { x, y, x, b };
                    moves.append(m);
                    if( TYPE(board[x][b]) != T_EMPTY )
                        break;
                    b += sign;
                }
            }
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

            // diag eat
            for(int sign=-1 ; sign<=1 ; sign+=2)
                if( inBounds(x+sign, y+orientation) ){
                    char tile = board[x+sign][y+orientation];
                    if( TYPE(tile) != T_EMPTY && COLOR(tile) != color ) {
                        Move m3 = { x, y, x+sign, y+orientation };
                        moves.append(m3);
                    } else if( enpassant && TYPE(tile) == T_EMPTY && enpassant_x == x+sign && enpassant_y == y ) {
                        Move m3 = { x, y, x+sign, y+orientation };
                        m3.enpassant = true;
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
    enpassant = false; // reset the enpassant status

    // we assume its legal
    board[move.to_x][move.to_y] = board[move.from_x][move.from_y];
    board[move.from_x][move.from_y] = T_EMPTY;

    bool white = COLOR(board[move.to_x][move.to_y]) == C_WHITE;

    if( TYPE(board[move.to_x][move.to_y]) == T_PAWN ){
        // pawn promotion @TODO
        if( move.to_y%7 == 0 )
            board[move.to_x][move.to_y] = T_QUEEN | COLOR(board[move.to_x][move.to_y]);

        // move is en passant: we need to remove the corresponding pawn
        if( move.enpassant )
            board[enpassant_x][enpassant_y] = T_EMPTY;

        // en passant will be available
        if( abs(move.from_y-move.to_y) == 2 ){
            enpassant = true;
            enpassant_x = move.to_x;
            enpassant_y = move.to_y;
        }

    } else if( TYPE(board[move.to_x][move.to_y]) == T_KING ){
        // if castling, need to move the rook
        if( abs(move.from_x-move.to_x) == 2 ){
            int sign = (move.from_x-move.to_x)/2;
            board[move.to_x+sign][move.to_y] = board[(1-sign)*7/2][move.to_y];
            board[(1-sign)*7/2][move.to_y] = T_EMPTY;
        }

        // king moved: cant castle anymore
        can_castle[white ? 0 : 1][0] = false;
        can_castle[white ? 0 : 1][1] = false;

    } else if( TYPE(board[move.to_x][move.to_y]) == T_ROOK) {
        // rook moved: cant castle with this one anymore
        if( move.from_y%7 == 0 && move.from_x%7 == 0 )
            can_castle[white ? 0 : 1][move.from_x/7] = false;
    }
}
