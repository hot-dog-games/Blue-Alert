#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "Summoner.h"
#include "CardManager.h"
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

Entity* EntityManager::CreateEntity(EntityType type, fPoint position, Card* card)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());

	id += "_" + card->name;

	DynamicEntity* entity = new DynamicEntity(entity_node, position, card);
	entities.push_back(entity);

	id_count++;

	return entity;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint position)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());

	id += "_CORE";

	StaticEntity* entity = new StaticEntity(entity_node, position);
	entities.push_back(entity);

	id_count++;

	return entity;
}

Summoner* EntityManager::CreateSummoner(Deck * deck)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)NONE).c_str());

	id += "_SUMMONER";

	Summoner* entity = new Summoner();
	entity->SetDeck(deck);
	entity->SetMaxEnergy(entity_node.attribute("energy").as_uint());
	entities.push_back(entity);

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
