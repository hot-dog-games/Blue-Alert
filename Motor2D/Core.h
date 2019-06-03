#ifndef _CORE_H_
#define _CORE_H_

#include <map>
#include "p2Defs.h"
#include "PerfTimer.h"
#include "StaticEntity.h"

class Deck;
struct Card;
class LeveledUpgrade;

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
	void LoadUnitSprites(int deck_size = 4);

	Stat* GetEnergy() const;
	Stat* GetHealth() const;
	void DecreaseEnergy(uint value);

	bool delete_deck = false;

protected:
	void Attack();
	bool CheckEnemies();
	Deck* deck = nullptr;
	PerfTimer energy_timer;
	PerfTimer attack_timer;
	Entity* objective;
	std::string attack_fx;

	bool CanUseCard(int card_num);
};

#endif // _CORE_H_




