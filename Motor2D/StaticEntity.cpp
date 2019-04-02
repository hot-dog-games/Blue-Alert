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

}


StaticEntity::~StaticEntity()
{
	LOG("destructor called");
}

bool StaticEntity::Update(float dt)
{
	if (state == STATIC_DIE && animations[state].isDone())
	{
		state = STATIC_DESTROYED;
	}

	current_frame = animations[state].GetCurrentFrame(dt);

	return true;
}

bool StaticEntity::CleanUp()
{
	if(sprite)
		App->tex->UnLoad(sprite);

	return true;
}

bool StaticEntity::PostUpdate()
{
	App->render->Blit(sprite, position.x, position.y, &current_frame);

	return true;
}


void StaticEntity::Die()
{
	state = STATIC_DIE;
}

