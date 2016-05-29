#include "card.h"

Card::Card(int nr, char suit)
    : m_nr{nr}
    , m_suit{suit}
{

}


std::ostream& operator << (std::ostream& valja, Card& c){
    if(c.getNr()>1 && c.getNr()<11) return valja << c.getNr()
                                                 << " " << c.getSuit();
    else if(c.getNr() == 1) return valja << "A " << c.getSuit();
    else if(c.getNr() == 11) return valja << "J " << c.getSuit();
    else if(c.getNr() == 12) return valja << "Q " << c.getSuit();
    else if(c.getNr() == 13) return valja << "K " << c.getSuit();
    return valja << "None";
}

int Card::getRank(){
    if(m_nr == 1) return 14;
    else return m_nr;
}

QString Card::toQString(){
    QString result;
    if(m_nr == 1) result = QString("A");
    else if(m_nr == 11) result = QString("J");
    else if(m_nr == 12) result = QString("Q");
    else if(m_nr == 13) result = QString("K");
    else result = QString::number(m_nr);
    return result;
}

bool Card::operator <(const Card& card)const{
    if(m_nr == card.getNr())
        return (m_suit < card.getSuit());
    if(m_nr == 1 || card.getNr()==1) return (card.getNr() < m_nr);
    return (m_nr < card.getNr());
}

