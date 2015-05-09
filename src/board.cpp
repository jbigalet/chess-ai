#include <QDebug>
#include "board.h"

Board::Board() {
    memset(state.board, 0, 64);
    memset(state.can_castle, 1, 4);
}

void Board::loadDefaultPosition() {
    for( int color=0 ; color<2 ; color++ ){
        int r = color*7;
        char c = (color ? C_WHITE : C_BLACK);

        // pawns
        for( int i=0 ; i<8 ; i++ )
            state.board[i][1+color*5] = T_PAWN | c;

        state.board[0][r] = state.board[7][r] = T_ROOK | c;
        state.board[1][r] = state.board[6][r] = T_KNIGHT | c;
        state.board[2][r] = state.board[5][r] = T_BISHOP | c;
        state.board[3][r] = T_QUEEN | c;
        state.board[4][r] = T_KING | c;
    }
}

QVector<Move> Board::possibleMoves(int x, int y, bool check){
    QVector<Move> moves;

    char color = COLOR(state.board[x][y]);
    bool queen = false; // to easily handle queen = bishop + rook

    switch(TYPE(state.board[x][y])) {
        case T_EMPTY:
            return moves;

        case T_KING:
            for(int i=-1 ; i<=1 ; i++)
                for(int j=-1 ; j<=1 ; j++)
                    if( (i!=0 || j!=0 ) && canGo(x+i, y+j, color) ){
                        Move m = { x, y, x+i, y+j };
                        moves.append(m);
                    }

            // castling
            for(int i=0 ; i<=1 ; i++) // left then right
                if( state.can_castle[color == C_WHITE ? 0 : 1][i] ){
                    bool open = true;
                    for(int j=1+5*i ; j!=4 ; j+=1-2*i)
                        if( state.board[j][y] != T_EMPTY ){
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
                        if( TYPE(state.board[a][b]) != T_EMPTY )
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
                    if( TYPE(state.board[a][y]) != T_EMPTY )
                        break;
                    a += sign;
                }

                // columns
                int b=y+sign;
                while( canGo(x, b, color) ){
                    Move m = { x, y, x, b };
                    moves.append(m);
                    if( TYPE(state.board[x][b]) != T_EMPTY )
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
            if( TYPE(state.board[x][y+orientation]) == T_EMPTY ){
                Move m = { x, y, x, y+orientation };
                moves.append(m);

                // 2nd row: can move 2
                if( (y-orientation)%7 == 0  && TYPE(state.board[x][y+2*orientation]) == T_EMPTY ){
                    Move m2 = { x, y, x, y+2*orientation };
                    moves.append(m2);
                }
            }

            // diag eat
            for(int sign=-1 ; sign<=1 ; sign+=2)
                if( inBounds(x+sign, y+orientation) ){
                    char tile = state.board[x+sign][y+orientation];
                    if( TYPE(tile) != T_EMPTY && COLOR(tile) != color ) {
                        Move m3 = { x, y, x+sign, y+orientation };
                        moves.append(m3);
                    } else if( state.enpassant && TYPE(tile) == T_EMPTY && state.enpassant_x == x+sign && state.enpassant_y == y ) {
                        Move m3 = { x, y, x+sign, y+orientation };
                        m3.enpassant = true;
                        moves.append(m3);
                    }
                }
            break;
    }

    // filter moves to remove the one putting the king in check
    // TODO atm dumb way: go through all the next possible move & check if one can take the king
    if( check ){
        QVector<Move> filtered_moves;

        foreach(Move move, moves){
            save();
            applyMove(move);
            if( !inCheck(color) )
                filtered_moves.append(move);
            load();
        }

        // special case for king: if castling, the castling move only still exists if
        // the 1 case towards the rook exists (unable to castle if in check during the king's travel)
        // + need not to be in check
        if( TYPE(state.board[x][y]) == T_KING )
            for(int index=0 ; index<filtered_moves.size() ; index++){
                Move move = filtered_moves[index];
                if( abs(move.to_x-move.from_x) == 2 ){
                    int target = move.from_x + (move.to_x-move.from_x)/2;
                    bool possible = false;
                    foreach(Move m, filtered_moves)
                        if( m.to_x == target && m.to_y == move.to_y ){
                            possible = true;
                            break;
                        }

                    if( !possible || inCheck(color) ){
                        filtered_moves.remove(index);
                        index--;
                    }
                }
            }

        return filtered_moves;
    }

    return moves;
}

QVector<Move> Board::allPossibleMoves(bool check){
    QVector<Move> moves;
    for(int i=0 ; i<8 ; i++)
        for(int j=0 ; j<8 ; j++)
            foreach(Move move, possibleMoves(i, j, check))
                moves.append(move);
    return moves;
}

bool Board::inCheck(char color){
    foreach(Move m, allPossibleMoves(false))
        if( state.board[m.to_x][m.to_y] == (T_KING | color) )
            return true;
    return false;
}

bool Board::canGo(int x, int y, char color){
    return inBounds(x, y) && isEatable(state.board[x][y], color);
}

void Board::applyMove(Move move){
    state.enpassant = false; // reset the state.enpassant status

    // we assume its legal
    state.board[move.to_x][move.to_y] = state.board[move.from_x][move.from_y];
    state.board[move.from_x][move.from_y] = T_EMPTY;

    bool white = COLOR(state.board[move.to_x][move.to_y]) == C_WHITE;

    if( TYPE(state.board[move.to_x][move.to_y]) == T_PAWN ){
        // pawn promotion @TODO
        if( move.to_y%7 == 0 )
            state.board[move.to_x][move.to_y] = T_QUEEN | COLOR(state.board[move.to_x][move.to_y]);

        // move is en passant: we need to remove the corresponding pawn
        if( move.enpassant )
            state.board[state.enpassant_x][state.enpassant_y] = T_EMPTY;

        // en passant will be available
        if( abs(move.from_y-move.to_y) == 2 ){
            state.enpassant = true;
            state.enpassant_x = move.to_x;
            state.enpassant_y = move.to_y;
        }

    } else if( TYPE(state.board[move.to_x][move.to_y]) == T_KING ){
        // if castling, need to move the rook
        if( abs(move.from_x-move.to_x) == 2 ){
            int sign = (move.from_x-move.to_x)/2;
            state.board[move.to_x+sign][move.to_y] = state.board[(1-sign)*7/2][move.to_y];
            state.board[(1-sign)*7/2][move.to_y] = T_EMPTY;
        }

        // king moved: cant castle anymore
        state.can_castle[white ? 0 : 1][0] = false;
        state.can_castle[white ? 0 : 1][1] = false;

    } else if( TYPE(state.board[move.to_x][move.to_y]) == T_ROOK) {
        // rook moved: cant castle with this one anymore
        if( move.from_y%7 == 0 && move.from_x%7 == 0 )
            state.can_castle[white ? 0 : 1][move.from_x/7] = false;
    }
}

void Board::save(){
    saved_state = state;
}

void Board::load(){
    state = saved_state;
}
