#include "holdemhandevaluator.h"

HoldemHandEvaluator::HoldemHandEvaluator()
{

}

int HoldemHandEvaluator::evaluate(Hand hand){
    hand.sort_hand();
    std::vector<Card*> cards = hand.getCards();
    vector<Hand> _5cards;
    int i;
    for(i=0; i<6; i++){
        int j;
        for(j=i+1; j<7; j++){
            Card* removed1 = cards.at(i);
            Card* removed2 = cards.at(j);
            hand.Remove_card(j);
            hand.Remove_card(i);
            _5cards.push_back(Hand(hand));
            hand.Add_card(removed1);
            hand.Add_card(removed2);
            hand.sort_hand();
        }

    }
    vector<int> scores;
    PokerHandEvaluator PHE;
    for (std::vector<Hand>::iterator it = _5cards.begin(); it != _5cards.end(); ++it){
        scores.push_back(PHE.evaluate(*it));
    }

    return *max_element(scores.begin(), scores.end());
}
