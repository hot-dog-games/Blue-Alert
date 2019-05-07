#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "p2Defs.h"
#include "Module.h"

struct Card;
class Deck;
class EncounterTree;
class StrategyBuilding;

enum EntityType;

class GameManager : public Module
{
public:
	GameManager();
	~GameManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool CleanUp();
	bool Load(pugi::xml_node&) { return true; }
	bool Save(pugi::xml_node&) const { return true; }

	EncounterTree* GetEncounterTree();
	Deck* GetPlayerDeck();

	int gold = 0;
	
	bool IsInPlayerDeck(Card* card);

	//----Initialization----

	void CreatePlayerDeck();

	//----------------------

	//---Collection_Acces----

	Card* GetCardFromCollection(EntityType card_type);
	void AddCardToCollection(EntityType card_type);
	std::list<Card*> collection;
	//----------------------
	bool Restart();
	bool restart = false;

private:
	Deck* combat_deck;

	EncounterTree* encounter_tree = nullptr;
};

#endif // !_GAME_MANAGER_H_