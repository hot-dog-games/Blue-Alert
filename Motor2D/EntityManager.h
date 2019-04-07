#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "p2Point.h"

class Entity;
class Core;
struct Card;
class Deck;
class StrategyBuilding;
enum Faction;

enum EntityType {
	NONE = -1,
	CORE,
	G_I,
	SNIPER,
	NAVY_SEAL,
	GRIZZLY,
	ROBOT,
	PRISM,
	NIGHTHAWK,
	HARRIER,
	BLACK_EAGLE,
	TESTSTRATEGYBUILDING
	//...
};

class EntityManager : public Module
{
public:
	std::list<Entity*> entities;
	EntityManager();
	~EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PreUpdate();
	bool PostUpdate();

	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Entity* CreateEntity(EntityType type, fPoint position, Card* card, Faction faction);
	Core* CreateCore(EntityType type, fPoint position, Deck* deck, Faction faction);
	StrategyBuilding* CreateStrategyBuilding(EntityType type, fPoint position, Faction faction);
	fPoint GetCorePosition(Faction faction);
	bool DeleteEntity(Entity* entity);
	void FindClosestEnemy(fPoint position, Faction faction, Entity* &closest_entity, float &distance);
	void FindClosestAllie(fPoint position, Faction faction, Entity* &closest_entity, float &distance);

private:
	pugi::xml_document entity_file;
	pugi::xml_node entity_configs;
	int id_count = 0;
};

#endif //_ENTITYMANAGER_H_