#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "DynamicEntity.h"
#include "CardManager.h"
#include "Core.h"
#include "Deck.h"
#include "EntityManager.h"



EntityManager::EntityManager()
{
	name = "entity_manager";
}

EntityManager::~EntityManager()
{
}

bool EntityManager::Awake(pugi::xml_node &)
{
	pugi::xml_parse_result result = entity_file.load_file("xml/entities.xml");

	if (result == NULL)
		LOG("Could not load entity xml. pugi error: %s", result.description());
	else
		entity_configs = entity_file.child("config");

	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->Update(dt);
	}

	return true;
}

bool EntityManager::PreUpdate()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->PreUpdate();
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->PostUpdate();
	}

	return true;
}

bool EntityManager::CleanUp()
{
	LOG("entity manager cleanup");

	while (!entities.empty()) delete entities.front(), entities.pop_front();

	entities.clear();
	id_count = 0;

	return true;
}

bool EntityManager::Load(pugi::xml_node&)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node&) const
{
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint position, Card* card, Faction faction)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());

	id += "_" + card->name;

	DynamicEntity* entity = new DynamicEntity(entity_node, position, card, faction);
	entities.push_back(entity);

	id_count++;

	return entity;
}

Core* EntityManager::CreateCore(EntityType type, fPoint position, Deck* deck, Faction faction)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());

	id += "_CORE";

	Core* entity = new Core(entity_node, position, faction);
	entities.push_back(entity);
	entity->SetDeck(deck);

	id_count++;

	return entity;
}

bool EntityManager::DeleteEntity(Entity* entity)
{
	entity->CleanUp();
	entities.remove(entity);
	delete entity;

	return true;
}
