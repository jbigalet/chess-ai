#ifndef DEF_GAME
#define DEF_GAME 

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QVector>
#include <QPoint>
#include "board.h"
#include "player.h"

class Game: public QWidget {
    public:
        Game();
        ~Game();

        void init();
        int inPossibleMove(int x, int y);
        void paintEvent(QPaintEvent*);
        void mouseReleaseEvent(QMouseEvent*);

        Board board;
        QPixmap pieces;

        bool selection_mode = false;
        int selection_x;
        int selection_y;
        QVector<Move> possible_moves;

        Player* white_player;
        Player* black_player;
};

#endif /* DEF_GAME */
