#include "blackjackhandevaluator.h"

BlackjackHandEvaluator::BlackjackHandEvaluator()
{
}

int BlackjackHandEvaluator::evaluate(Hand hand){
    std::vector<Card*> cards = hand.getCards();
    int aces = 0;
    int value = 0;
    for (std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); ++it){
        if((*it)->getNr() > 1 && (*it)->getNr() <11) value += (*it)->getNr();
        else if((*it)->getNr() >10) value += 10;
        else aces++;
    }
    if(aces == 0) return value;
    int best = value+aces*1;
    if(best > 21) return best;
    for(int i = 0; i<aces; i++){
        int temp_value = value+i*1+(aces-i)*11;
        if(temp_value <= 21 && temp_value > best) best = temp_value;
    }
    return best;
}
