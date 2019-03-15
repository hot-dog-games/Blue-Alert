#ifndef _DECK_H_
#define _DECK_H_

#include <vector>

struct Card;

class Deck
{
public:
	Deck();
	~Deck();

	void AddCard();
	void RemoveCard();
	void UpgradeCard();
	//TODO:: Get cards from deck.

private:
	std::vector<Card*> card_list; //Maybe this should be array with fixed size?
	//TODO: A way to differentiate cards used in combat from the others.
};

#endif // _DECK_H_



