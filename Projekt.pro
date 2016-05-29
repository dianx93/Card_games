#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T10:51:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projekt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    card.cpp \
    deck.cpp \
    hand.cpp \
    blackjackhandevaluator.cpp \
    pokerhandevaluator.cpp \
    gamewidget.cpp

HEADERS  += mainwindow.h \
    card.h \
    deck.h \
    hand.h \
    blackjackhandevaluator.h \
    pokerhandevaluator.h \
    gamewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    pics/spade.png \
    pics/club.png \
    pics/diamond.png \
    pics/heart.png \
    pics/card_back_2.png

DISTFILES +=
