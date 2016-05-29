#ifndef BLACKJACKHANDEVALUATOR_H
#define BLACKJACKHANDEVALUATOR_H

#include "hand.h"
#include "card.h"
#include <vector>

class BlackjackHandEvaluator
{
public:
    BlackjackHandEvaluator();
    int evaluate(Hand hand);
};

#endif // BLACKJACKHANDEVALUATOR_H
