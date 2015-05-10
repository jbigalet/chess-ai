#include <QApplication>
#include "game.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Game game;
    game.show();
    game.init();
    return app.exec();
}
