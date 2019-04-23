#ifndef _CARD_MANAGER_H_
#define _CARD_MANAGER_H_

#include <map>

#include "p2Defs.h"
#include "Module.h"
#include "Animation.h"
#include "EntityManager.h"

class Stat;
class SDL_Texture;

enum AttackType {
	BASIC,
	AOE,
	PIERCING
};

struct CardScaling {
	uint health_upgrade;
	uint attack_damage_upgrade;
	uint defense_upgrade;
	uint movement_speed_upgrade;
	uint attack_speed_upgrade;
	uint range_upgrade;
};
struct CardInfo {
	AttackType attack_type;
	CardScaling scaling;

	std::map<std::string, Stat*> stats;
	bool armored;


};

struct Card {
	EntityType type;
	CardInfo info;

	uint level;
	std::string name = " ";
	std::string sprite_path = " ";
	SDL_Texture* texture = nullptr;

	bool to_delete = false;

	void Upgrade();
	void LoadSprite();
};

class CardManager : public Module
{
public:
	CardManager();
	~CardManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool CleanUp();
	bool PostUpdate();
	bool Load(pugi::xml_node&) { return true; }
	bool Save(pugi::xml_node&) const { return true; }

	Card* CreateCard(EntityType type);
	Card* DeleteCard(Card* card);

private:
	void LoadCardStats(Card* card, pugi::xml_node stats_node);
	void LoadCardUpgrades(Card* card, pugi::xml_node upgrades_node);
	void LoadCardCombat(Card* card, pugi::xml_node combat_node);

private:
	bool to_delete = false;
	pugi::xml_document config_file;
	pugi::xml_node card_configs;
	std::list<Card*> cards;
};

#endif // _CARD_MANAGER_H_


