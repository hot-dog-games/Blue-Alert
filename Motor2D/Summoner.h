#ifndef _SUMMONER_H_
#define _SUMMONER_H_

#include "Entity.h"

class Deck;

enum CardNumber{
	CN_FIRST,
	CN_SECOND,
	CN_THIRD,
	CN_FOURTH
};

class Summoner : public Entity
{
public:
	Summoner();
	~Summoner();
	
	bool CleanUp();
	void UseCard(CardNumber number, fPoint position);
	void SetDeck(Deck* new_deck);
	void SetMaxEnergy(uint energy);

private:
	void DecreaseEnergy(uint energy);

private:
	Deck* deck;

	uint max_energy;
	uint current_energy;
	uint energy_regen;
};

#endif // !_SUMMONER_H_


