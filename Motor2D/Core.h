#ifndef _CORE_H_
#define _CORE_H_

#include <map>

#include "p2Defs.h"
#include "StaticEntity.h"

class Deck;
class Stat;


class Core : public StaticEntity
{
public:
	enum class CardNumber {
		FIRST,
		SECOND,
		THIRD,
		FOURTH
	};

public:
	Core(pugi::xml_node entity_config, fPoint position);
	~Core();

	bool CleanUp();

	void UseCard(CardNumber number, fPoint position);
	void SetDeck(Deck* new_deck);


private:
	Deck* deck;

	std::map<std::string, Stat*> stats;
	uint current_energy;
};

#endif // _CORE_H_




