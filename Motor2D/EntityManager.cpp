#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "DynamicEntity.h"
#include "StaticEntity.h"
#include "CardManager.h"
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
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		draw_path = !draw_path;

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
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->CleanUp();
		entity = entities.erase(entity);
	}
	entities.clear();
	id_count = 0;

	return true;
}

bool EntityManager::Load(pugi::xml_node &)
{
	return true;
}

bool EntityManager::Save(pugi::xml_node &) const
{
	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint position, Card* card)
{
	std::string id = std::to_string(id_count);
	switch (type)
	{
	case EntityType::G_I:
	{
		id += "_" + card->name;
		DynamicEntity* entity = new DynamicEntity();
		((DynamicEntity*)entity)->SetCard(card);
		entities.push_back(entity);
		return entity;
	}
		break;
	case EntityType::CORE:
	{
		id += "_CORE";
		StaticEntity* entity = new StaticEntity();

		// Prepare entity 
		entity->position = position;
		pugi::xml_node entity_node = entity_configs.child("static").find_child_by_attribute("type", std::to_string((int)type).c_str());
		entity->LoadAnimations(entity_node);
		entity->SetMaxLife(entity_node.attribute("life").as_uint());
		entity->LoadSprite(entity_node.child("sprite").child_value());

		entities.push_back(entity);

		return entity;
	}
		break;
	}

	id_count++;

	return nullptr;
}

bool EntityManager::DeleteEntity(Entity * entity)
{
	entities.remove(entity);
	return true;
}
