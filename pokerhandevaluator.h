#ifndef POKERHANDEVALUATOR_H
#define POKERHANDEVALUATOR_H

#include "hand.h"
#include "card.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <math.h>

#include <iostream>

using namespace std;

class PokerHandEvaluator
{
public:
    PokerHandEvaluator();
    int evaluate(Hand hand);

    bool isFlush(vector<Card*> cards);
    bool isStraight(vector<Card*> cards);
    bool is4s(vector<Card*> cards);
    bool isFullHouse(vector<Card*> cards);
    bool is3s(vector<Card*> cards);
    bool is22s(vector<Card*> cards);
    bool is2s(vector<Card*> cards);

    int valueStraightFlush(vector<Card*> cards);
    int valueFourOfAKind(vector<Card*> cards);
    int valueFullHouse(vector<Card*> cards);
    int valueFlush(vector<Card*> cards);
    int valueStraight(vector<Card*> cards);
    int valueSet(vector<Card*> cards);
    int valueTwoPairs(vector<Card*> cards);
    int valueOnePair(vector<Card*> cards);
    int valueHighCard(vector<Card*> cards);
};

#endif // POKERHANDEVALUATOR_H
