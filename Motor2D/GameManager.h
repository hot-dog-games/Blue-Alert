#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "p2Defs.h"
#include "Module.h"
#include <map>

struct Card;
class Deck;
class EncounterTree;
class StrategyBuilding;
class BuffSource;
class Core;
class Stat;

enum EntityType;

enum stage {
	STAGE_TUTORIAL,
	STAGE_01
};

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
	void LevelUpgrade();

	int gold = 300;
	std::map<std::string, Stat*> stats;
	int stage = STAGE_TUTORIAL;
	
	bool IsInPlayerDeck(Card* card);

	//----Initialization----

	void CreatePlayerDeck();
	void CreateStage();
	void CreateUpgrades();

	//----------------------

	//---Collection_Acces----

	Card* GetCardFromCollection(EntityType card_type);
	void AddCardToCollection(EntityType card_type);
	bool IsInCollection(int card_type);
	std::list<Card*> collection;
	//----------------------
	bool Restart();
	bool restart = false;
	void ClearUpgrades();
	BuffSource* GetUpgrade(EntityType unit_type);

	void UpgradeHealth();
	void UpgradeEnergy();

	void CreateCoreStats();
	BuffSource* health_upgrade = nullptr;
	BuffSource* energy_upgrade = nullptr;
private:
	Deck* combat_deck;

	//Maybe put this bitch in a vector?
	BuffSource* infantry_upgrade = nullptr;
	BuffSource* land_upgrade = nullptr;
	BuffSource* aerial_upgrade = nullptr;

	EncounterTree* encounter_tree = nullptr;
};

#endif // !_GAME_MANAGER_H_