#ifndef CARD_H
#define CARD_H

#include <QObject>
#include <QString>
#include <ostream>

class Card : public QObject
{
    Q_OBJECT
    int m_nr;
    char m_suit;
public:
    explicit Card(int nr = 0, char suit = '?');
    inline int getNr() const{return m_nr;}
    inline char getSuit()const{return m_suit;}
    QString toQString();
    int getRank();
    bool operator < (const Card& card)const;

signals:

public slots:
};
std::ostream& operator << (std::ostream& valja, Card& c);
#endif // CARD_H
