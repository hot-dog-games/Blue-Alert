#ifndef _DECK_H_
#define _DECK_H_

struct Card;

const uint MAX_CARDS = 4;

class Deck
{
public:
	Deck();
	~Deck();

	void CleanUp();
	void AddCard(Card* card);
	void RemoveCard(uint position);

	Card* cards[MAX_CARDS];

	bool delete_cards = false;
};

#endif // _DECK_H_



