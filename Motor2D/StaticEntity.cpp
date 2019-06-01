#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "StaticEntity.h"



StaticEntity::StaticEntity()
{
	state = STATIC_IDLE;
}

StaticEntity::StaticEntity(pugi::xml_node entity_node, fPoint position, Faction faction) : Entity(entity_node, position, faction)
{
	LoadSprite(entity_node);
}


StaticEntity::~StaticEntity()
{
	LOG("destructor called");
}

bool StaticEntity::CleanUp()
{
	App->tex->UnLoad(sprite);

	return true;
}


void StaticEntity::Die()
{
	state = STATIC_DIE;
	current_animation = &animations.find("die")->second;
}

void StaticEntity::LoadSprite(pugi::xml_node node)
{
	std::string sprite_path = node.child("sprite").child_value();
	sprite = App->tex->Load(sprite_path.c_str());
}