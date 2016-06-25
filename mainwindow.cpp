#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blackjackhandevaluator.h"
#include "pokerhandevaluator.h"
#include "gamewidget.h"
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    std::srand (unsigned (std::time(0)));
    _setmode(_fileno(stdout), _O_WTEXT);
    setFixedSize(800, 600);
    initWidgets();
    initMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenus(){
    m_newGameMenu = new QMenu(this);
    m_newGameMenu->setTitle(QString("&New Game"));
    menuBar()->addMenu(m_newGameMenu);

    //Blackjack action
    m_BlackjackAction = new QAction(this);
    m_BlackjackAction->setText(QString("&Blackjack"));
    m_BlackjackAction->setStatusTip(QString("Starts a game of Blackjack"));
    m_BlackjackAction->setToolTip(QString("Starts a game of Blackjack"));
    m_BlackjackAction->setWhatsThis(QString("Activate this to start a game of Blackjack."));

    m_newGameMenu->addAction(m_BlackjackAction);

    connect(m_BlackjackAction, SIGNAL(triggered()),
            this, SLOT(playBlackjack()));

    //Poker action
    m_PokerAction = new QAction(this);
    m_PokerAction->setText(QString("&Poker"));
    m_PokerAction->setStatusTip(QString("Starts a game of Poker"));
    m_PokerAction->setToolTip(QString("Starts a game of Poker"));
    m_PokerAction->setWhatsThis(QString("Activate this to start a game of Poker."));

    m_newGameMenu->addAction(m_PokerAction);

    connect(m_PokerAction, SIGNAL(triggered()),
            this, SLOT(playPoker()));

    //Holdem action
    m_HoldemAction = new QAction(this);
    m_HoldemAction->setText(QString("&Texas hold'em"));
    m_HoldemAction->setStatusTip(QString("Starts a game of Texas hold'em"));
    m_HoldemAction->setToolTip(QString("Starts a game of Texas hold'em"));
    m_HoldemAction->setWhatsThis(QString("Activate this to start a game of Texas hold'em."));

    m_newGameMenu->addAction(m_HoldemAction);

    connect(m_HoldemAction, SIGNAL(triggered()),
            this, SLOT(playHoldem()));
}

void MainWindow::initWidgets(){
    setWindowTitle(QString("Card games"));

    m_gameWidget = new GameWidget(this);
    setCentralWidget(m_gameWidget);
}

void MainWindow::playBlackjack(){
    m_gameWidget->setGameName("Blackjack");
    Deck deck;
    Hand player;
    Hand computer;
    player.Add_card(deck.Draw());
    player.Add_card(deck.Draw());
    m_gameWidget->setState(GameWidgetState::B_PLAYERS_TURN);
    m_gameWidget->setPlayerHand(player);
    m_gameWidget->setComputerHand(computer);
    m_gameWidget->setDeck(deck);
    m_gameWidget->update();
}

void MainWindow::playPoker(){
    m_gameWidget->setGameName("Poker");
    Deck deck;
    Hand player;
    Hand computer;
    player.Add_card(deck.Draw());
    player.Add_card(deck.Draw());
    player.Add_card(deck.Draw());
    player.Add_card(deck.Draw());
    player.Add_card(deck.Draw());
    player.sort_hand();
    m_gameWidget->setState(GameWidgetState::P_PLAYERS_TURN);
    m_gameWidget->setPlayerHand(player);
    m_gameWidget->setComputerHand(computer);
    m_gameWidget->setDeck(deck);
    m_gameWidget->update();
}

void MainWindow::playHoldem(){
    m_gameWidget->setGameName("Texas hold'em");
    Deck deck;
    Hand player;
    Hand computer;
    player.Add_card(deck.Draw());
    player.Add_card(deck.Draw());
    computer.Add_card(deck.Draw());
    computer.Add_card(deck.Draw());
    m_gameWidget->setState(GameWidgetState::H_PLAYER_DRAWS);
    m_gameWidget->setPlayerHand(player);
    m_gameWidget->setComputerHand(computer);
    m_gameWidget->setDeck(deck);
    m_gameWidget->update();
}

void MainWindow::setNewGameEnabled(bool b){
    m_newGameMenu->setEnabled(b);
}
