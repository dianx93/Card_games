#include "gamewidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QPixmap>
#include <QBrush>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include "mainwindow.h"

GameWidget::GameWidget(MainWindow *parent)
    : QWidget(parent), m_mainWindow(parent)
{
    setMouseTracking(true);
    std::fill_n(swapping, 5, false);
}

GameWidget::~GameWidget() {
    // Nothing here yet
}

/**
 * Pictures from:
 * http://icons.iconarchive.com/icons/designcontest/casino/96/Spade-icon.png
 * http://www.iconarchive.com/show/casino-icons-by-designcontest/Hearts-icon.html
 * http://www.iconarchive.com/show/casino-icons-by-designcontest/Diamonds-icon.html
 * http://www.iconarchive.com/show/casino-icons-by-designcontest/Club-icon.html
 */

void GameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::green);

    //Game name
    QFont font;
    font.setPixelSize(40);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::red);
    painter.drawText(100, 10, 600, 50, Qt::AlignCenter, game_name);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);

    if(game_name == QString("Blackjack")){
        //computer's hand
        font.setPixelSize(12);
        font.setBold(false);
        font.setCapitalization(QFont::MixedCase);
        painter.setFont(font);
        vector<Card*> computercards = m_computer_hand.getCards();
        if(!computercards.empty()) painter.drawText(40, 80, 150, 16, Qt::AlignLeft, QString("Computer's hand:"));
        int left_start = 40;
        QPixmap suit_pic;
        for (std::vector<Card*>::iterator it = computercards.begin(); it != computercards.end(); ++it){
            painter.drawRoundedRect(left_start, 100, 55, 77, 5, 5);
            font.setBold(true);
            font.setPixelSize(14);
            painter.setFont(font);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
            if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
            else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
            else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
            else suit_pic = QPixmap(":/pics/club.png");
            painter.drawText(left_start+7, 105, 45, 67, Qt::AlignLeft, (**it).toQString());
            painter.drawPixmap(left_start+12, 128, 30, 30, suit_pic);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            left_start += 65;
        }

        //player's hand
        font.setBold(false);
        font.setPixelSize(12);
        painter.setFont(font);
        vector<Card*> playercards = m_player_hand.getCards();
        if(!playercards.empty()) painter.drawText(40, 420, 150, 16, Qt::AlignLeft, QString("Your hand:"));
        left_start = 40;
        for (std::vector<Card*>::iterator it = playercards.begin(); it != playercards.end(); ++it){
            painter.drawRoundedRect(left_start, 440, 55, 77, 5, 5);
            font.setBold(true);
            font.setPixelSize(14);
            painter.setFont(font);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
            if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
            else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
            else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
            else suit_pic = QPixmap(":/pics/club.png");
            painter.drawText(left_start+7, 445, 45, 67, Qt::AlignLeft, (**it).toQString());
            painter.drawPixmap(left_start+12, 468, 30, 30, suit_pic);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            left_start += 65;
        }

        //deck
        QBrush brush(QPixmap(":/pics/card_back_2.png"));
        painter.drawRoundedRect(238, 242, 55, 77, 5, 5);
        painter.drawRoundedRect(236, 244, 55, 77, 5, 5);
        painter.drawRoundedRect(234, 246, 55, 77, 5, 5);
        painter.drawRoundedRect(232, 248, 55, 77, 5, 5);
        painter.setBrush(brush);
        painter.drawRoundedRect(230, 250, 55, 77, 5, 5);

        //player's score
        if(m_state == B_PLAYERS_TURN || m_state == B_PLAYER_BUSTED || m_state == B_PLAYER_WIN ||
                m_state == B_COMPUTERS_TURN || m_state == B_COMPUTER_BUSTED || m_state == B_COMPUTER_WIN){
            painter.drawText(300, 330, 200, 20, Qt::AlignCenter, QString("Your score:"));
            painter.drawText(300, 350, 200, 20, Qt::AlignCenter, QString::number(BHE.evaluate(m_player_hand)));
        }
        //computer's score
        if(m_state == B_PLAYER_WIN || m_state == B_COMPUTERS_TURN || m_state == B_COMPUTER_BUSTED ||
                m_state == B_COMPUTER_WIN){
            painter.drawText(300, 200, 200, 20, Qt::AlignCenter, QString("Computer's score:"));
            painter.drawText(300, 220, 200, 20, Qt::AlignCenter, QString::number(BHE.evaluate(m_computer_hand)));
        }
        if(m_state == B_PLAYER_WIN || m_state == B_PLAYER_BUSTED || m_state == B_COMPUTER_BUSTED ||
                m_state == B_COMPUTER_WIN){
            painter.setBrush(Qt::white);
            painter.drawRoundedRect(500, 260, 100, 55, 5, 5);
            painter.drawText(510, 265, 80, 20, Qt::AlignCenter, QString("PLAY"));
            painter.drawText(510, 285, 80, 20, Qt::AlignCenter, QString("AGAIN"));
        }
        if(m_state == B_PLAYERS_TURN){
            //text
            font.setPixelSize(13);
            painter.setFont(font);
            painter.drawText(300, 255, 200, 20, Qt::AlignCenter, QString("Draw a card from"));
            painter.drawText(300, 275, 200, 20, Qt::AlignCenter, QString("the deck or"));
            painter.drawText(300, 295, 200, 20, Qt::AlignCenter, QString("end your turn!"));
            //end turn
            painter.setBrush(Qt::white);
            painter.drawRoundedRect(500, 260, 55, 55, 5, 5);
            painter.drawText(510, 265, 35, 20, Qt::AlignCenter, QString("END"));
            painter.drawText(510, 285, 35, 20, Qt::AlignCenter, QString("TURN"));
        }
        else if(m_state == B_PLAYER_BUSTED){
            painter.setPen(Qt::red);
            painter.drawText(300, 255, 200, 20, Qt::AlignCenter, QString("YOU BUSTED"));
            painter.drawText(300, 285, 200, 20, Qt::AlignCenter, QString("COMPUTER WINS!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == B_COMPUTERS_TURN){
            painter.drawText(300, 275, 200, 20, Qt::AlignCenter, QString("COMPUTER'S TURN"));
        }
        else if(m_state == B_COMPUTER_BUSTED){
            painter.setPen(Qt::blue);
            painter.drawText(300, 255, 200, 20, Qt::AlignCenter, QString("COMPUTER BUSTED"));
            painter.drawText(300, 285, 200, 20, Qt::AlignCenter, QString("YOU WIN!!!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == B_COMPUTER_WIN){
            painter.setPen(Qt::red);
            painter.drawText(300, 270, 200, 20, Qt::AlignCenter, QString("COMPUTER WINS!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == B_PLAYER_WIN){
            painter.setPen(Qt::blue);
            painter.drawText(300, 270, 200, 20, Qt::AlignCenter, QString("YOU WIN!!!"));
            painter.setPen(Qt::black);
        }
    }
    else if(game_name == QString("Poker")){
        //computer's hand
        font.setPixelSize(12);
        font.setBold(false);
        font.setCapitalization(QFont::MixedCase);
        painter.setFont(font);
        vector<Card*> computercards = m_computer_hand.getCards();
        if(!computercards.empty()) painter.drawText(10, 120, 100, 16, Qt::AlignRight, QString("Computer's hand:"));
        int left_start = 120;
        QPixmap suit_pic;
        int diff;
        for (std::vector<Card*>::iterator it = computercards.begin(); it != computercards.end(); ++it){
            diff = 0;
            if(m_state == P_COMPUTER_SWAPPING && swapping[it-computercards.begin()]) diff = 20;
            painter.drawRoundedRect(left_start, 100+diff, 80, 112, 5, 5);
            font.setBold(true);
            font.setPixelSize(20);
            painter.setFont(font);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
            if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
            else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
            else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
            else suit_pic = QPixmap(":/pics/club.png");
            painter.drawText(left_start+10, 107+diff, 82, 122, Qt::AlignLeft, (**it).toQString());
            painter.drawPixmap(left_start+17, 141+diff, 44, 44, suit_pic);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            left_start += 90;
        }

        //player's hand
        font.setBold(false);
        font.setPixelSize(12);
        painter.setFont(font);
        vector<Card*> playercards = m_player_hand.getCards();
        if(!playercards.empty()) painter.drawText(10, 420, 100, 16, Qt::AlignRight, QString("Your hand:"));
        left_start = 120;
        for (std::vector<Card*>::iterator it = playercards.begin(); it != playercards.end(); ++it){
            diff = 0;
            if(m_state == P_PLAYERS_TURN && swapping[it-playercards.begin()]) diff = 20;
            painter.drawRoundedRect(left_start, 400-diff, 80, 112, 5, 5);
            font.setBold(true);
            font.setPixelSize(20);
            painter.setFont(font);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
            if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
            else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
            else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
            else suit_pic = QPixmap(":/pics/club.png");
            painter.drawText(left_start+10, 407-diff, 82, 122, Qt::AlignLeft, (**it).toQString());
            painter.drawPixmap(left_start+17, 441-diff, 44, 44, suit_pic);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            left_start += 90;
        }

        //deck
        QBrush brush(QPixmap(":/pics/card_back_2.png"));
        painter.drawRoundedRect(610, 240, 80, 112, 5, 5);
        painter.drawRoundedRect(608, 242, 80, 112, 5, 5);
        painter.drawRoundedRect(606, 244, 80, 112, 5, 5);
        painter.drawRoundedRect(604, 246, 80, 112, 5, 5);
        painter.drawRoundedRect(602, 248, 80, 112, 5, 5);
        painter.setBrush(brush);
        painter.drawRoundedRect(600, 250, 80, 112, 5, 5);

        if(m_state == P_PLAYERS_TURN){
            font.setPixelSize(13);
            painter.setFont(font);
            painter.drawText(200, 255, 300, 20, Qt::AlignCenter, QString("Choose the cards you wish to"));
            painter.drawText(200, 275, 300, 20, Qt::AlignCenter, QString("replace and click the deck to"));
            painter.drawText(200, 295, 300, 20, Qt::AlignCenter, QString("replace them and end your turn"));
        }
        else if(m_state == P_PLAYER_SORTING){
            font.setPixelSize(13);
            painter.setFont(font);
            painter.drawText(200, 275, 300, 20, Qt::AlignCenter, QString("Sorting your"));
            painter.drawText(200, 295, 300, 20, Qt::AlignCenter, QString("cards"));
        }
        else if(m_state == P_COMPUTER_SORTING){
            font.setPixelSize(13);
            painter.setFont(font);
            painter.drawText(200, 275, 300, 20, Qt::AlignCenter, QString("Computer is sorting"));
            painter.drawText(200, 295, 300, 20, Qt::AlignCenter, QString("cards"));
        }
        else if(m_state == P_COMPUTER_SWAPPING){
            font.setPixelSize(13);
            painter.setFont(font);
            painter.drawText(200, 275, 300, 20, Qt::AlignCenter, QString("Computer is swapping"));
            painter.drawText(200, 295, 300, 20, Qt::AlignCenter, QString("cards"));
        }
        else if(m_state == P_COMPUTER_WIN){
            font.setPixelSize(25);
            painter.setFont(font);
            painter.setPen(Qt::red);
            painter.drawText(200, 265, 300, 30, Qt::AlignCenter, QString("COMPUTER WINS!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == P_PLAYER_WIN){
            font.setPixelSize(25);
            painter.setFont(font);
            painter.setPen(Qt::blue);
            painter.drawText(200, 265, 300, 30, Qt::AlignCenter, QString("YOU WIN!!!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == P_TIE){
            font.setPixelSize(25);
            painter.setFont(font);
            painter.setPen(Qt::darkBlue);
            painter.drawText(200, 265, 300, 30, Qt::AlignCenter, QString("IT'S A TIE!"));
            painter.setPen(Qt::black);
        }
        if(m_state == P_PLAYER_WIN || m_state == P_COMPUTER_WIN || m_state == P_TIE){
            painter.setBrush(Qt::gray);
            font.setPixelSize(14);
            painter.setFont(font);
            painter.drawRoundedRect(300, 305, 100, 55, 5, 5);
            painter.drawText(310, 310, 80, 20, Qt::AlignCenter, QString("PLAY"));
            painter.drawText(310, 330, 80, 20, Qt::AlignCenter, QString("AGAIN"));
        }
    }
    else if(game_name == QString("Texas hold'em")){
        //computer's hand
        font.setPixelSize(12);
        font.setBold(false);
        font.setCapitalization(QFont::MixedCase);
        painter.setFont(font);
        vector<Card*> computercards = m_computer_hand.getCards();
        if(!computercards.empty()) painter.drawText(10, 120, 100, 16, Qt::AlignRight, QString("Computer's hand:"));
        int left_start = 120;
        QPixmap suit_pic;
        QBrush back(QPixmap(":/pics/card_back_2.png"));
        painter.setBrush(Qt::white);
        for (std::vector<Card*>::iterator it = computercards.begin(); it != computercards.end(); ++it){
            if(m_state!=H_PLAYER_WIN && m_state!=H_COMPUTER_WIN && m_state != H_TIE){
                painter.setBrush(back);
                painter.drawRoundedRect(left_start, 100, 80, 112, 5, 5);
                painter.setBrush(Qt::white);
            }
            else{
                painter.drawRoundedRect(left_start, 100, 80, 112, 5, 5);
                font.setBold(true);
                font.setPixelSize(20);
                painter.setFont(font);
                if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
                if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
                else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
                else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
                else suit_pic = QPixmap(":/pics/club.png");
                painter.drawText(left_start+10, 107, 82, 122, Qt::AlignLeft, (**it).toQString());
                painter.drawPixmap(left_start+17, 141, 44, 44, suit_pic);
                if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            }
            left_start += 90;
        }

        //player's hand
        font.setBold(false);
        font.setPixelSize(12);
        painter.setFont(font);
        vector<Card*> playercards = m_player_hand.getCards();
        if(!playercards.empty()) painter.drawText(10, 420, 100, 16, Qt::AlignRight, QString("Your hand:"));
        left_start = 120;
        for (std::vector<Card*>::iterator it = playercards.begin(); it != playercards.end(); ++it){
            painter.drawRoundedRect(left_start, 400, 80, 112, 5, 5);
            font.setBold(true);
            font.setPixelSize(20);
            painter.setFont(font);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
            if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
            else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
            else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
            else suit_pic = QPixmap(":/pics/club.png");
            painter.drawText(left_start+10, 407, 82, 122, Qt::AlignLeft, (**it).toQString());
            painter.drawPixmap(left_start+17, 441, 44, 44, suit_pic);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            left_start += 90;
        }

        //deck
        painter.drawRoundedRect(610, 240, 80, 112, 5, 5);
        painter.drawRoundedRect(608, 242, 80, 112, 5, 5);
        painter.drawRoundedRect(606, 244, 80, 112, 5, 5);
        painter.drawRoundedRect(604, 246, 80, 112, 5, 5);
        painter.drawRoundedRect(602, 248, 80, 112, 5, 5);
        painter.setBrush(back);
        painter.drawRoundedRect(600, 250, 80, 112, 5, 5);
        painter.setBrush(Qt::white);

        //board
        font.setBold(false);
        font.setPixelSize(12);
        painter.setFont(font);
        vector<Card*> boardcards = m_board.getCards();
        left_start = 120;
        for (std::vector<Card*>::iterator it = boardcards.begin(); it != boardcards.end(); ++it){
            painter.drawRoundedRect(left_start, 260, 80, 112, 5, 5);
            font.setBold(true);
            font.setPixelSize(20);
            painter.setFont(font);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::red);
            if((**it).getSuit() == 'D') suit_pic = QPixmap(":/pics/diamond.png");
            else if((**it).getSuit() == 'H') suit_pic = QPixmap(":/pics/heart.png");
            else if((**it).getSuit() == 'S') suit_pic = QPixmap(":/pics/spade.png");
            else suit_pic = QPixmap(":/pics/club.png");
            painter.drawText(left_start+10, 267, 82, 122, Qt::AlignLeft, (**it).toQString());
            painter.drawPixmap(left_start+17, 301, 44, 44, suit_pic);
            if((**it).getSuit() == 'D' || (**it).getSuit() == 'H') painter.setPen(Qt::black);
            left_start += 90;
        }

        //buttons
        if(m_state != H_PLAYER_WIN && m_state != H_COMPUTER_WIN && m_state != H_TIE && m_state != H_DOING_FLOP){
            font.setBold(false);
            painter.setBrush(Qt::white);
            painter.drawRoundedRect(330, 400, 100, 40, 5, 5);
            font.setPixelSize(20);
            painter.setFont(font);
            painter.drawText(330, 410, 100, 20, Qt::AlignCenter, QString("Continue"));
        }
        else if(m_state == H_COMPUTER_WIN){
            font.setPixelSize(25);
            painter.setFont(font);
            painter.setPen(Qt::red);
            painter.drawText(330, 165, 300, 30, Qt::AlignCenter, QString("COMPUTER WINS!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == H_PLAYER_WIN){
            font.setPixelSize(25);
            painter.setFont(font);
            painter.setPen(Qt::blue);
            painter.drawText(330, 165, 300, 30, Qt::AlignCenter, QString("YOU WIN!!!"));
            painter.setPen(Qt::black);
        }
        else if(m_state == H_TIE){
            font.setPixelSize(25);
            painter.setFont(font);
            painter.setPen(Qt::darkBlue);
            painter.drawText(330, 165, 300, 30, Qt::AlignCenter, QString("IT'S A TIE!"));
            painter.setPen(Qt::black);
        }
        if(m_state == H_PLAYER_WIN || m_state == H_COMPUTER_WIN || m_state == H_TIE){
            painter.setBrush(Qt::gray);
            font.setPixelSize(14);
            painter.setFont(font);
            painter.drawRoundedRect(440, 420, 100, 55, 5, 5);
            painter.drawText(450, 425, 80, 20, Qt::AlignCenter, QString("PLAY"));
            painter.drawText(450, 445, 80, 20, Qt::AlignCenter, QString("AGAIN"));
        }
    }
    else{
        painter.setBrush(Qt::gray);
        font.setPixelSize(20);
        painter.setFont(font);
        painter.setBrush(Qt::white);
        painter.drawRoundedRect(250, 150, 300, 40, 5, 5);
        painter.drawRoundedRect(250, 200, 300, 40, 5, 5);
        painter.drawRoundedRect(250, 250, 300, 40, 5, 5);
        painter.setPen(Qt::red);
        painter.drawText(250, 160, 300, 20, Qt::AlignCenter, QString("PLAY BLACKJACK"));
        painter.drawText(250, 210, 300, 20, Qt::AlignCenter, QString("PLAY POKER"));
        painter.drawText(250, 260, 300, 20, Qt::AlignCenter, QString("PLAY TEXAS HOLD'EM"));
        font.setPixelSize(15);
        painter.setPen(Qt::darkRed);
        painter.setFont(font);
        painter.drawText(200, 400, 400, 20, Qt::AlignCenter, QString("WARNING: These games are addictive!"));
        painter.drawText(200, 420, 400, 20, Qt::AlignCenter, QString("Play at your own risk!"));
    }
}

//http://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
void GameWidget::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void GameWidget::mousePressEvent(QMouseEvent *event) {
    if(game_name == QString("")){
        if(event->button() == Qt::LeftButton){
            if(event->button() == Qt::LeftButton) {
                if(event->x()>250 && event->x()<550 && event->y()>150 && event->y()<190){
                    m_mainWindow->playBlackjack();
                }
                else if(event->x()>250 && event->x()<550 && event->y()>200 && event->y()<240){
                    m_mainWindow->playPoker();
                }
                else if(event->x()>250 && event->x()<550 && event->y()>250 && event->y()<290){
                    m_mainWindow->playHoldem();
                }
            }
        }
    }
    else if(m_state == B_PLAYERS_TURN){
        if(event->button() == Qt::LeftButton) {
            if(event->x()>230 && event->x()<285 && event->y()>250 && event->y()<327){
                m_player_hand.Add_card(m_deck.Draw());
                if(BHE.evaluate(m_player_hand)>21){
                    m_state = B_PLAYER_BUSTED;
                }
            }
            else if(event->x()>500 && event->x()<555 && event->y()>260 && event->y()<315){
                m_state = B_COMPUTERS_TURN;
                m_mainWindow->setNewGameEnabled(false);
                update();
                delay(1000);
                m_computer_hand.Add_card(m_deck.Draw());
                while(BHE.evaluate(m_computer_hand) < 17){
                    update();
                    delay(1000);
                    m_computer_hand.Add_card(m_deck.Draw());
                }
                m_mainWindow->setNewGameEnabled(true);
                if(BHE.evaluate(m_computer_hand) > 21){
                    m_state = B_COMPUTER_BUSTED;
                }
                else if(BHE.evaluate(m_computer_hand)==21 &&
                        BHE.evaluate(m_player_hand)==21){
                    if(m_computer_hand.getCards().size()!=2){
                        m_state = B_PLAYER_WIN;
                    }
                    else if(m_player_hand.getCards().size()!=2 && m_computer_hand.getCards().size()==2){
                        m_state = B_COMPUTER_WIN;
                    }
                }
                else if(BHE.evaluate(m_computer_hand)>BHE.evaluate(m_player_hand)){
                    m_state = B_COMPUTER_WIN;
                }
                else m_state = B_PLAYER_WIN;
            }
        }
    }
    else if(m_state == B_PLAYER_WIN || m_state == B_PLAYER_BUSTED || m_state == B_COMPUTER_BUSTED ||
            m_state == B_COMPUTER_WIN){
        if(event->button() == Qt::LeftButton) {
            if(event->x()>500 && event->x()<600 && event->y()>260 && event->y()<315){
                m_mainWindow->playBlackjack();
            }
        }
    }
    else if(m_state == P_PLAYERS_TURN){
        if(event->button() == Qt::LeftButton) {
            int diff;
            int i = 0;
            for(bool t : swapping){
                if(t) diff = 20;
                else diff = 0;
                if(event->x()>i*90+120 && event->x()<i*90+200 && event->y()>400-diff && event->y()<512-diff){
                    swapping[i] = !swapping[i];
                    update();
                }
                i++;
            }
            if(event->x()>600 && event->x()<690 && event->y()>240 && event->y()<362){
                m_mainWindow->setNewGameEnabled(false);
                swap(m_player_hand);
                m_state = P_PLAYER_SORTING;
                update();
                delay(2000);
                m_player_hand.sort_hand();
                m_state = P_COMPUTER_DRAWING;
                for(int i = 0; i<5; i++){
                    update();
                    delay(1000);
                    m_computer_hand.Add_card(m_deck.Draw());
                }
                m_state = P_COMPUTER_SORTING;
                update();
                delay(2000);
                m_computer_hand.sort_hand();
                m_state = P_COMPUTER_SWAPPING;
                update();
                delay(2000);
                pokerAI(m_computer_hand);
                update();
                m_state = P_COMPUTER_SORTING;
                delay(2000);
                m_computer_hand.sort_hand();
                update();
                delay(1500);
                //winner:
                if(PHE.evaluate(m_computer_hand) > PHE.evaluate(m_player_hand)){
                    m_state = P_COMPUTER_WIN;
                }
                else if(PHE.evaluate(m_player_hand) > PHE.evaluate(m_computer_hand)){
                    m_state = P_PLAYER_WIN;
                }
                else m_state = P_TIE;
                m_mainWindow->setNewGameEnabled(true);
            }
        }
    }
    else if(m_state == P_PLAYER_WIN || m_state == P_COMPUTER_WIN || m_state == P_TIE){
        if(event->button() == Qt::LeftButton) {
            if(event->x()>300 && event->x()<400 && event->y()>305 && event->y()<360){
                m_mainWindow->playPoker();
            }
        }
    }
    else if(m_state == H_DRAWS || m_state == H_FLOP || m_state == H_TURN || m_state == H_RIVER){
        if(event->button() == Qt::LeftButton) {
            if(event->x()>330 && event->x()<430 && event->y()>400 && event->y()<440){
                if(m_state == H_DRAWS){
                    m_state = H_DOING_FLOP;
                    m_mainWindow->setNewGameEnabled(false);
                    m_board.Add_card(m_deck.Draw());
                    update();
                    delay(800);
                    m_board.Add_card(m_deck.Draw());
                    update();
                    delay(800);
                    m_board.Add_card(m_deck.Draw());
                    m_state = H_FLOP;
                    m_mainWindow->setNewGameEnabled(true);
                }
                else if(m_state == H_FLOP){
                    m_state = H_TURN;
                    m_board.Add_card(m_deck.Draw());
                }
                else if(m_state == H_TURN){
                    m_state = H_RIVER;
                    m_board.Add_card(m_deck.Draw());
                }
                else if(m_state == H_RIVER){
                    if(HHE.evaluate(m_computer_hand, m_board) > HHE.evaluate(m_player_hand, m_board)){
                        m_state = H_COMPUTER_WIN;
                    }
                    else if(HHE.evaluate(m_player_hand, m_board) > HHE.evaluate(m_computer_hand, m_board)){
                        m_state = H_PLAYER_WIN;
                    }
                    else m_state = H_TIE;
                }
            }
        }
    }
    else if(m_state == H_PLAYER_WIN || m_state == H_COMPUTER_WIN || m_state == H_TIE){
        if(event->button() == Qt::LeftButton) {
            if(event->x()>440 && event->x()<540 && event->y()>420 && event->y()<475){
                m_mainWindow->playHoldem();
            }
        }
    }
    update();
}

void GameWidget::swap(Hand& hand){
    for(int i = 4; i>=0; i--){
        if(swapping[i]){
            hand.Remove_card(i);
            hand.Add_card(m_deck.Draw());
        }
    }
    std::fill_n(swapping, 5, false);
}


void GameWidget::pokerAI(Hand& computer){
    std::vector<Card*> cards = computer.getCards();
    if(PHE.evaluate(computer)<1000000){
        int i = 0;
        for (std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); ++it){
            if((**it).getRank()<10) setToSwap(i);
            i++;
        }
    }
    else if(PHE.evaluate(computer)<2000000){
        if ((*cards[0]).getRank() == (*cards[1]).getRank()){
            if((*cards[2]).getRank() < 11) setToSwap(2);
            if((*cards[3]).getRank() < 11) setToSwap(3);
            if((*cards[4]).getRank() < 11) setToSwap(4);
        }
        else if ((*cards[1]).getRank() == (*cards[2]).getRank()){
            if((*cards[0]).getRank() < 11) setToSwap(0);
            if((*cards[3]).getRank() < 11) setToSwap(3);
            if((*cards[4]).getRank() < 11) setToSwap(4);
        }
        else if ((*cards[2]).getRank() == (*cards[3]).getRank()){
            if((*cards[0]).getRank() < 11) setToSwap(0);
            if((*cards[1]).getRank() < 11) setToSwap(1);
            if((*cards[4]).getRank() < 11) setToSwap(4);
        }
        else {
            if((*cards[0]).getRank() < 11) setToSwap(0);
            if((*cards[1]).getRank() < 11) setToSwap(1);
            if((*cards[2]).getRank() < 11) setToSwap(2);
        }
    }
    else if(PHE.evaluate(computer)<3000000){
        if ((*cards[0]).getRank() == (*cards[1]).getRank() &&
                (*cards[2]).getRank() == (*cards[3]).getRank())
            setToSwap(4);
        else if ((*cards[0]).getRank() == (*cards[1]).getRank() &&
                 (*cards[3]).getRank() == (*cards[4]).getRank())
            setToSwap(2);
        else
            setToSwap(0);
    }
    else if(PHE.evaluate(computer)<4000000){
        if((*cards[0]).getRank()==(*cards[1]).getRank() &&
                (*cards[1]).getRank()==(*cards[2]).getRank()){
            if((*cards[3]).getRank() < 11) setToSwap(3);
            if((*cards[4]).getRank() < 11) setToSwap(4);
        }
        if((*cards[1]).getRank()==(*cards[2]).getRank() &&
                (*cards[2]).getRank()==(*cards[3]).getRank()){
            if((*cards[0]).getRank() < 11) setToSwap(0);
            if((*cards[4]).getRank() < 11) setToSwap(4);
        }
        if((*cards[2]).getRank()==(*cards[3]).getRank() &&
                (*cards[3]).getRank()==(*cards[4]).getRank()){
            if((*cards[0]).getRank() < 11) setToSwap(0);
            if((*cards[1]).getRank() < 11) setToSwap(1);
        }
    }
    update();
    delay(2000);
    swap(computer);
}

void GameWidget::setToSwap(int i){
    swapping[i] = true;
    update();
    delay(1000);
}

