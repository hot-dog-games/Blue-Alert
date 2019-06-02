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
	STAGE_NONE = -1,
	STAGE_TUTORIAL,
	STAGE_01
};

enum tutorial_popup {
	POPUP_BUILDING_NODES,
	POPUP_USETROOP,
	POPUP_SNIPER_COUNTERS,
	POPUP_AREA_COUNTERS,
	POPUP_MULTIPLE_COUNTERS,
	POPUP_ARMORED_COUNTERS,
	POPUP_DECISIONMAKING,
	POPUP_STORE,
	POPUP_REWARD_TROOP,
	POPUP_STRATEGY_MAPMENU,
	POPUP_TROOPS_MENU,
	POPUP_BUILDINGS_MENU,
	POPUP_TUTORIAL_END,
	POPUP_MAX
};

struct CardState {
	int lvl = 0;
	EntityType type;
};

struct GameState
{
	int stage = -1;
	int node = 0;
	std::list<int> captured_nodes;

	EntityType deck_state[4];
	std::list<CardState> collection_state;
	int health_lvl = 0;
	int energy_lvl = 0;
	int gold = 0;
};

class GameManager : public Module
{
public:
	GameManager();
	~GameManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	EncounterTree* GetEncounterTree();
	Deck* GetPlayerDeck();
	void LevelUpgrade(EntityType);

	int gold = 0;
	std::map<std::string, Stat*> stats;
	int stage = STAGE_TUTORIAL;

	int enemy_scaling = 0;
	
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
	//----------------------

	bool Restart();
	bool restart = false;
	void ClearUpgrades();
	BuffSource* GetUpgrade(EntityType unit_type);
	void SaveRecoveryState();

	void UpgradeHealth();
	void UpgradeEnergy();
	void ResetBuildingBuffs();

	void CreateCoreStats();
	BuffSource* health_upgrade = nullptr;
	BuffSource* energy_upgrade = nullptr;

	void CreatePopUps();
	void ShowPopUp(int popup);

	bool popups[POPUP_MAX];

private:
	void RecoverState(GameState state);
	void SaveState(GameState &state) const;
	void StateToXML(GameState &state, pugi::xml_node&) const;
	void XMLToState(GameState &state, pugi::xml_node&);

private:
	Deck* combat_deck = nullptr;
	std::list<Card*> collection;

public:
	//Not good like this but w/e
	BuffSource* infantry_upgrade = nullptr;
	BuffSource* land_upgrade = nullptr;
	BuffSource* aerial_upgrade = nullptr;

	mutable GameState recovery_state;
	mutable GameState save_state;
private:

	EncounterTree* encounter_tree = nullptr;
};

#endif // !_GAME_MANAGER_H_