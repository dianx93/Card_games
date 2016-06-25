#ifndef HOLDEMHANDEVALUATOR_H
#define HOLDEMHANDEVALUATOR_H

#include "hand.h"
#include "card.h"
#include "pokerhandevaluator.h"
#include <iostream>
#include <algorithm>

using namespace std;

class HoldemHandEvaluator
{
public:
    HoldemHandEvaluator();
    int evaluate(Hand hand);

};

#endif // HOLDEMHANDEVALUATOR_H
