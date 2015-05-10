QT += core gui widgets

CONFIG += console c++11

TARGET = chess
TEMPLATE = app

SOURCES += main.cpp \
           game.cpp \
           board.cpp \
           humanPlayer.cpp \
           AIPlayer.cpp

HEADERS  += game.h \
            board.h \
            player.h \
            humanPlayer.h \
            AIPlayer.h
