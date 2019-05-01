#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "DynamicEntity.h"
#include "StrategyBuilding.h"
#include "CardManager.h"
#include "Core.h"
#include "CoreAI.h"
#include "Deck.h"
#include "Map.h"
#include "EntityManager.h"
#include "Brofiler/Brofiler.h"

bool higher_y(Entity* first, Entity* second)
{
	return (first->position.y < second->position.y && first->sorting_layer <= second->sorting_layer);
}

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
	if (!paused) 
	{
		BROFILER_CATEGORY("EMUpdate", Profiler::Color::Plum);

		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_UP)
			SetDebug();

		for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
		{
			(*entity)->Update(dt);
		}

	}

	return true;
}

bool EntityManager::PreUpdate()
{
	if (!paused)
	{
		for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
		{
			(*entity)->PreUpdate();
		}
	}

	return true;
}

bool EntityManager::PostUpdate()
{
	entities.sort(higher_y);

	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if(App->render->IsOnCamera((*entity)->position.x - (*entity)->current_frame.w*0.5f, (*entity)->position.y - (*entity)->current_frame.h,
			(*entity)->current_frame.w, (*entity)->current_frame.h))
			(*entity)->PostUpdate();
	}

	return true;
}

bool EntityManager::CleanUp()
{
	LOG("entity manager cleanup");

	while (!entities.empty()) 
	{
		entities.front()->CleanUp();
		delete entities.front();
		entities.pop_front();
	}

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

bool EntityManager::Pause()
{
	paused = true;

	return true;
}

bool EntityManager::Resume()
{
	paused = false;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint position, Card* card, Faction faction)
{
	BROFILER_CATEGORY("CreateEntity", Profiler::Color::Gold);

	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());

	id += "_" + card->name;
	
	DynamicEntity* entity = new DynamicEntity(entity_node, position, card, faction);
	entity->type = type;
	entity->Start();
	entity->SetDebug(debug);

	if (type == EntityType::HARRIER || type == EntityType::NIGHTHAWK || type == EntityType::BLACK_EAGLE)
		entity->sorting_layer = 1;

	entities.push_back(entity);
	id_count++;

	return entity;
}

Core* EntityManager::CreateCore(uint core_type, fPoint position, Deck* deck, Faction faction, bool ai)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type", std::to_string((int)CORE).c_str());
	pugi::xml_node stats_node = entity_node.child("stats");
	pugi::xml_node info;

	if (faction == FACTION_AMERICAN)
	{
		info = entity_node.find_child_by_attribute("faction", std::to_string((int)FACTION_AMERICAN).c_str()).find_child_by_attribute("core_type", std::to_string((int)core_type).c_str());
	}
	else {
		info = entity_node.find_child_by_attribute("faction", std::to_string((int)FACTION_RUSSIAN).c_str());
	}

	id += "_CORE";
	Core* entity;
	if (ai)
	{
		entity = new CoreAI(info, position, faction, stats_node);
	}
	else
	{
		entity = new Core(info, position, faction, stats_node);
	}
	entities.push_back(entity);
	entity->type = CORE;
	entity->Start();
	entity->SetDeck(deck);
	entity->SetDebug(debug);

	id_count++;

	return entity;
}

StrategyBuilding* EntityManager::CreateStrategyBuilding(EntityType type, fPoint position, Faction faction)
{
	std::string id = std::to_string(id_count);
	pugi::xml_node entity_node = entity_configs.find_child_by_attribute("type",
		std::to_string((int)type).c_str()).find_child_by_attribute("faction", std::to_string((int)faction).c_str());

	id += "_TESTSTRATEGYBUILDING";

	StrategyBuilding* entity = new StrategyBuilding(entity_node, position, faction);
	entities.push_back(entity);
	entity->Start();

	id_count++;

	return entity;
}

fPoint EntityManager::GetCorePosition(Faction faction)
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if((*entity)->faction != faction && (*entity)->type == CORE)
		{
			return (*entity)->position;
		}
	}
	return {0,0};
}

void EntityManager::FindClosestEnemy(fPoint position, Faction faction, Entity* &closest_entity, float &distance)
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->faction != faction && (*entity)->IsAlive())
		{
			float tmp_distance = position.DistanceTo((*entity)->position);
			if (tmp_distance < distance)
			{
				distance = tmp_distance;
				closest_entity = (*entity);
			}
		}
	}
}

void EntityManager::FindClosestAllie(fPoint position, Faction faction, Entity* &closest_entity, float &distance)
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->faction == faction && (*entity)->IsAlive() && (*entity)->position != position)
		{
			float tmp_distance = position.DistanceTo((*entity)->position);
			if (tmp_distance < distance)
			{
				distance = tmp_distance;
				closest_entity = (*entity);
			}
		}
	}
}

void EntityManager::GetEntitiesInArea(SDL_Rect area, std::list<Entity*> &list, int faction)
{
	Entity* ent;
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		ent = (*entity);
		if (ent->type != CORE && ent->type != faction && ent->IsAlive() 
			&& (ent->position.x >= area.x && ent->position.x <= area.x + area.w 
				&& ent->position.y >= area.y && ent->position.y <= area.y + area.h))
			list.push_back(ent);
	}
}

void EntityManager::GetEntitiesInArea(float radius, fPoint position, std::list<Entity*> &list, int faction)
{
	Entity* ent;
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		ent = (*entity);
		float distance = position.DistanceTo(ent->position);
		if (ent->faction != faction && ent->IsAlive() 
			&& distance <= radius)
			list.push_back(ent);
	}
}

void EntityManager::SetDebug()
{
	debug = !debug;
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->SetDebug(debug);
	}
}

bool EntityManager::DeleteEntity(Entity* entity)
{
	entity->CleanUp();
	entities.remove(entity);
	delete entity;

	return true;
}

bool EntityManager::CreateGroup(int units, EntityType type, fPoint position, Card * card, Faction faction)
{
	for (int i = 0; i < units; i++)
	{
		if (i == 1)
		{
			position.x = position.x + 10;
			position.y = position.y + 10;
		}
		else if (i == 2)
		{
			position.x = position.x - 10;
			position.y = position.y + 10;
		}
		else if (i == 3)
		{
			position.y = position.y + 20;
		}
		App->entity_manager->CreateEntity(type, position, card, faction);

	}
	return true;
}

Entity* EntityManager::InThisTile_IsUnits(iPoint tile)
{
	Entity* ret = nullptr;
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		iPoint TilePos = App->map->WorldToMap((*entity)->position.x, (*entity)->position.y);
		if (TilePos == tile)
			return ret = *entity;
	}
	return ret;
}