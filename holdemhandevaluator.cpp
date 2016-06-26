#include "holdemhandevaluator.h"

HoldemHandEvaluator::HoldemHandEvaluator()
{

}

struct Compare{
    bool operator()(const Card* l, const Card* r){
        return *l < *r;
    }
};

int HoldemHandEvaluator::evaluate(Hand hand, Hand board){
    hand += board;
    vector<Card*> cards = hand.getCards();
    vector<Hand> _5cards;
    int i;
    for(i=0; i<6; i++){
        int j;
        for(j=i+1; j<7; j++){
            Card* removed1 = cards.at(i);
            Card* removed2 = cards.at(j);
            hand.Remove_card(j);
            hand.Remove_card(i);
            _5cards.push_back(hand);
            hand.Add_card(removed1, i);
            hand.Add_card(removed2, j);
        }

    }
    vector<int> scores;
    PokerHandEvaluator PHE;
    for (std::vector<Hand>::iterator it = _5cards.begin(); it != _5cards.end(); ++it){
        scores.push_back(PHE.evaluate(*it));
    }
    return *max_element(scores.begin(), scores.end());
}
