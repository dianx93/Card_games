#include "hand.h"

Hand::Hand()
{
}

void Hand::Add_card(Card* card){
    cards.push_back(card);
}

void Hand::Remove_card(int i){
    cards.erase(cards.begin() + i);
}

struct Compare{
    bool operator()(const Card* l, const Card* r){
        return *l < *r;
    }
};

void Hand::sort_hand(){
    std::sort(cards.begin(), cards.end(), Compare());
}


std::ostream& operator << (std::ostream& valja, Hand& h){
    std::vector<Card*> cards = h.getCards();
    for (std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); ++it){
        valja << **it << "; ";
    }
    return valja;
}