#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "p2Point.h"

class Entity;

enum EntityType {
	BUILDING,
	TROOP
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
	bool CreateEntity(EntityType type, fPoint position, Card* card = nullptr);
	bool DeleteEntity(Entity* entity);

private:
	pugi::xml_node entity_configs;
	int id_count = 0;
	bool draw_path = false;
};

#endif //_ENTITYMANAGER_H_