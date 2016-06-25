#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hand.h"
#include "deck.h"

class GameWidget;
class QMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setNewGameEnabled(bool b);
private:
    Ui::MainWindow *ui;
protected:
    void initMenus();
    void initWidgets();

    GameWidget *m_gameWidget;

    QMenu *m_newGameMenu;
        QAction *m_BlackjackAction;
        QAction *m_PokerAction;
        QAction *m_HoldemAction;

public slots:
    void playBlackjack();
    void playPoker();
    void playHoldem();

};

#endif // MAINWINDOW_H
