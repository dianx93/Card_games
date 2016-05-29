#ifndef DECK_H
#define DECK_H

#include <algorithm>
#include <vector>
#include "card.h"


class Deck
{
    std::vector<Card*> cards;
public:
    Deck();
    void Generate();
    void Shuffle();
    Card* Draw();
};

#endif // DECK_H
