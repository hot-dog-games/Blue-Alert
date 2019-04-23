#ifndef _CORE_H_
#define _CORE_H_

#include <map>
#include "p2Defs.h"
#include "PerfTimer.h"
#include "StaticEntity.h"

class Deck;
struct Card;

class Core : public StaticEntity
{
public:
	Core(pugi::xml_node entity_config, fPoint position, Faction faction, pugi::xml_node stats_node);
	~Core();

	virtual bool Update(float dt);

	virtual bool CleanUp();

	bool UseCard(int number, fPoint position);
	void SetDeck(Deck* new_deck);
	Card* GetCard(int card_num) const;
	void LoadUnitSprites();

	Stat* GetEnergy() const;
	Stat* GetHealth() const;

	bool delete_deck = false;

protected:
	Deck* deck = nullptr;
	PerfTimer energy_timer;

	bool CanUseCard(int card_num);

};

#endif // _CORE_H_




