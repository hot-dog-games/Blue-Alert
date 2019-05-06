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
	Card* GetCard(EntityType type);
	Card* cards[MAX_CARDS];

	int GetDeckSize() const;

	bool delete_cards = false;
};

#endif // _DECK_H_



