#include "deck.h"

Deck::Deck()
{
    Generate();
    Shuffle();
}

void Deck::Generate(){
    for(int i = 0; i<13; i++){
        cards.push_back(new Card(i+1, 'S'));
        cards.push_back(new Card(i+1, 'H'));
        cards.push_back(new Card(i+1, 'D'));
        cards.push_back(new Card(i+1, 'C'));
    }
}

int m_random (int i) {
    return std::rand()%i;
}

void Deck::Shuffle(){
    std::random_shuffle(cards.begin(), cards.end(), m_random);
}

Card* Deck::Draw(){
    Card* card = cards.back();
    cards.pop_back();
    return card;
}
