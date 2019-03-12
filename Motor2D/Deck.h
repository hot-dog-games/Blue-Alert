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

private:
	std::vector<Card*> card_list;
};

#endif // !_DECK_H_



