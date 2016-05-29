#ifndef HAND_H
#define HAND_H

#include <vector>
#include <ostream>
#include "card.h"

class Hand
{
    std::vector<Card*> cards;
public:
    Hand();
    void Add_card(Card* card);
    inline std::vector<Card*> getCards(){
        return cards;
    }
    void Remove_card(int i);
    void sort_hand();
};

std::ostream& operator << (std::ostream& valja, Hand& h);
#endif // HAND_H
