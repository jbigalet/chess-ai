QT += core gui widgets

CONFIG += console c++11

TARGET = chess
TEMPLATE = app

SOURCES += main.cpp \
           game.cpp \
           board.cpp

HEADERS  += game.h \
            board.h
