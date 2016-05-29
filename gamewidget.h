#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QString>
#include <vector>
#include "card.h"
#include "hand.h"
#include "deck.h"
#include "blackjackhandevaluator.h"
#include "pokerhandevaluator.h"

using namespace std;

class MainWindow;

enum GameWidgetState {
    B_PLAYERS_TURN,
    B_PLAYER_BUSTED,
    B_COMPUTERS_TURN,
    B_COMPUTER_BUSTED,
    B_COMPUTER_WIN,
    B_PLAYER_WIN,
    P_PLAYERS_TURN,
    P_PLAYER_SORTING,
    P_COMPUTER_DRAWING,
    P_COMPUTER_SORTING,
    P_COMPUTER_SWAPPING,
    P_PLAYER_WIN,
    P_COMPUTER_WIN,
    P_TIE
};

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    //explicit GameWidget(QWidget *parent = 0);
    GameWidget(MainWindow *parent = 0);
    ~GameWidget();

signals:

public slots:

    inline void setState(GameWidgetState state){
        m_state = state;
    }
    inline void setGameName(QString name){
        game_name = name;
    }
    inline void setPlayerHand(Hand hand){
        m_player_hand = hand;
    }
    inline void setComputerHand(Hand hand){
        m_computer_hand = hand;
    }
    inline void setDeck(Deck deck){
        m_deck = deck;
    }

protected:
    GameWidgetState m_state;
    MainWindow *m_mainWindow;

    Hand m_player_hand;
    Hand m_computer_hand;
    Deck m_deck;
    BlackjackHandEvaluator BHE;
    PokerHandEvaluator PHE;

    void delay( int millisecondsToWait );

    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    QString game_name;

    bool swapping[5];

    void swap(Hand& hand);
    void pokerAI(Hand& computer);
    void setToSwap(int i);

};

#endif // GAMEWIDGET_H
