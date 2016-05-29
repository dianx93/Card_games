#include "pokerhandevaluator.h"

PokerHandEvaluator::PokerHandEvaluator()
{

}

struct Compare{
    bool operator()(const Card* l, const Card* r){
        return *l < *r;
    }
};

int PokerHandEvaluator::evaluate(Hand hand){
    //logic and some code taken from here: http://www.mathcs.emory.edu/~cheung/Courses/170/Syllabus/10/pokerValue.html
    std::vector<Card*> cards = hand.getCards();
    std::sort(cards.begin(), cards.end(), Compare());

    if (isFlush(cards) && isStraight(cards)) return valueStraightFlush(cards);
    else if (is4s(cards)) return valueFourOfAKind(cards);
    else if (isFullHouse(cards)) return valueFullHouse(cards);
    else if (isFlush(cards)) return valueFlush(cards);
    else if (isStraight(cards)) return valueStraight(cards);
    else if (is3s(cards)) return valueSet(cards);
    else if (is22s(cards)) return valueTwoPairs(cards);
    else if (is2s(cards)) return valueOnePair(cards);
    else return valueHighCard(cards);   // Lowest Poker hand;


}

bool PokerHandEvaluator::isFlush(vector<Card*> cards){
    if((*cards[0]).getSuit()==(*cards[1]).getSuit() &&
            (*cards[1]).getSuit()==(*cards[2]).getSuit() &&
            (*cards[2]).getSuit()==(*cards[3]).getSuit() &&
            (*cards[3]).getSuit()==(*cards[4]).getSuit())
        return true;
    return false;
}

bool PokerHandEvaluator::isStraight(vector<Card*> cards){
    if((*cards[0]).getRank()+1==(*cards[1]).getRank() &&
            (*cards[1]).getRank()+1==(*cards[2]).getRank() &&
            (*cards[2]).getRank()+1==(*cards[3]).getRank() &&
            (*cards[3]).getRank()+1==(*cards[4]).getRank())
        return true;
    return false;
}

bool PokerHandEvaluator::is4s(vector<Card*> cards){
    if((*cards[1]).getRank()==(*cards[2]).getRank() &&
            (*cards[2]).getRank()==(*cards[3]).getRank() &&
            ((*cards[0]).getRank()==(*cards[1]).getRank() ||
            (*cards[3]).getRank()==(*cards[4]).getRank()))
        return true;
    return false;
}

bool PokerHandEvaluator::isFullHouse(vector<Card*> cards){
    if((*cards[0]).getRank()==(*cards[1]).getRank() &&
            ((*cards[1]).getRank()==(*cards[2]).getRank() ||
            (*cards[2]).getRank()==(*cards[3]).getRank()) &&
            (*cards[3]).getRank()==(*cards[4]).getRank())
        return true;
    return false;
}

bool PokerHandEvaluator::is3s(vector<Card*> cards){
    if((*cards[0]).getRank()==(*cards[1]).getRank() &&
            (*cards[1]).getRank()==(*cards[2]).getRank() ||
            (*cards[1]).getRank()==(*cards[2]).getRank() &&
            (*cards[2]).getRank()==(*cards[3]).getRank() ||
            (*cards[2]).getRank()==(*cards[3]).getRank() &&
            (*cards[3]).getRank()==(*cards[4]).getRank())
        return true;
    return false;
}

bool PokerHandEvaluator::is22s(vector<Card*> cards){
    if((*cards[0]).getRank()==(*cards[1]).getRank() &&
            ((*cards[2]).getRank()==(*cards[3]).getRank() ||
            (*cards[3]).getRank()==(*cards[4]).getRank()) ||
            (*cards[1]).getRank()==(*cards[2]).getRank() &&
            (*cards[3]).getRank()==(*cards[4]).getRank())
        return true;
    return false;
}

bool PokerHandEvaluator::is2s(vector<Card*> cards){
    if((*cards[0]).getRank()==(*cards[1]).getRank() ||
            (*cards[1]).getRank()==(*cards[2]).getRank() ||
            (*cards[2]).getRank()==(*cards[3]).getRank() ||
            (*cards[3]).getRank()==(*cards[4]).getRank())
        return true;
    return false;
}

int PokerHandEvaluator::valueStraightFlush(vector<Card*> cards){
    int interValue = 8000000;
    return interValue + valueHighCard(cards);
}

int PokerHandEvaluator::valueFourOfAKind(vector<Card*> cards){
    int interValue = 7000000;
    int intraValue = (*cards[2]).getRank();
    return interValue + intraValue;
}

int PokerHandEvaluator::valueFullHouse(vector<Card*> cards){
    int interValue = 6000000;
    int intraValue = (*cards[2]).getRank();
    return interValue + intraValue;
}

int PokerHandEvaluator::valueFlush(vector<Card*> cards){
    int interValue = 5000000;
    return interValue + valueHighCard(cards);
}

int PokerHandEvaluator::valueStraight(vector<Card*> cards){
    int interValue = 4000000;
    return interValue + valueHighCard(cards);
}

int PokerHandEvaluator::valueSet(vector<Card*> cards){
    int interValue = 3000000;
    int intraValue = (*cards[2]).getRank();
    return interValue + intraValue;
}

int PokerHandEvaluator::valueTwoPairs(vector<Card*> cards){
    int interValue = 2000000;
    int intraValue;
    if ((*cards[0]).getRank() == (*cards[1]).getRank() &&
            (*cards[2]).getRank() == (*cards[3]).getRank())
        // a a b b x
        intraValue = 14*14*(*cards[2]).getRank() +
                14*(*cards[0]).getRank() + (*cards[4]).getRank();
    else if ((*cards[0]).getRank() == (*cards[1]).getRank() &&
             (*cards[3]).getRank() == (*cards[4]).getRank())
        // a a x b b
        intraValue = 14*14*(*cards[3]).getRank() +
                14*(*cards[0]).getRank() + (*cards[2]).getRank();
    else
        // x a a b b
        intraValue = 14*14*(*cards[3]).getRank() +
                14*(*cards[1]).getRank() + (*cards[0]).getRank();
    return interValue + intraValue;
}

int PokerHandEvaluator::valueOnePair(vector<Card*> cards){
    int interValue = 1000000;
    int intraValue;
    if ((*cards[0]).getRank() == (*cards[1]).getRank())
        // a a x y z
        intraValue = 14*14*14*(*cards[0]).getRank() +
                (*cards[2]).getRank() + 14*(*cards[3]).getRank() +
                14*14*(*cards[4]).getRank();
    else if ((*cards[1]).getRank() == (*cards[2]).getRank())
        // x a a y z
        intraValue = 14*14*14*(*cards[1]).getRank() +
                (*cards[0]).getRank() + 14*(*cards[3]).getRank() +
                14*14*(*cards[4]).getRank();
    else if ((*cards[2]).getRank() == (*cards[3]).getRank())
        // x y a a z
        intraValue = 14*14*14*(*cards[2]).getRank() +
                (*cards[0]).getRank() + 14*(*cards[1]).getRank() +
                14*14*(*cards[4]).getRank();
    else
        // x y z a a
        intraValue = 14*14*14*(*cards[3]).getRank() +
                (*cards[0]).getRank() +
                14*(*cards[1]).getRank() + 14*14*(*cards[2]).getRank();
    return interValue + intraValue;
}

int PokerHandEvaluator::valueHighCard(vector<Card*> cards){
    int interValue = 0;
    int intraValue = 0;
    int i = 0;
    for (std::vector<Card*>::iterator it = cards.begin(); it != cards.end(); ++it){
        intraValue += (*it)->getRank()*(pow(14, i));
        i++;
    }
    return interValue + intraValue;
}
