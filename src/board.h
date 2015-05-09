#ifndef DEF_BOARD
#define DEF_BOARD 

#include <QVector>

// the board is defined by a char[8][8], where each char is a tile
// a tile is either empty or contains a piece:
//   - 4 lower bits: type
//   - next bit: color

#define T_EMPTY   0x00
#define T_KING    0x01
#define T_QUEEN   0x02
#define T_BISHOP  0x03
#define T_KNIGHT  0x04
#define T_ROOK    0x05
#define T_PAWN    0x06

#define C_WHITE   0x00
#define C_BLACK   0x10

#define TYPE(tile)  (tile & 0x0F)
#define COLOR(tile) (tile & 0x10)

typedef struct Move {
    Move() {};
    Move(int fx, int fy, int tx, int ty): from_x(fx), from_y(fy), to_x(tx), to_y(ty), enpassant(false) {};

    int from_x;
    int from_y;
    int to_x;
    int to_y;
    bool enpassant;
} Move;

class Board {
    public:
        Board();
        ~Board() {};

        void loadDefaultPosition();
        QVector<Move> possibleMoves(int x, int y);
        void applyMove(Move move);
        bool canGo(int x, int y, char color);

        char board[8][8];

        bool enpassant = false; // last move was a pawn by 2
        int enpassant_x;
        int enpassant_y;

        bool can_castle[2][2]; // white/black & left/right
};

inline bool inBounds(int x, int y){
    return x>=0 && x<8 && y>=0 && y<8;
}

inline bool isEatable(char tile, char color){
    return TYPE(tile) == T_EMPTY || COLOR(tile) != color;
}

#endif /* DEF_BOARD */
