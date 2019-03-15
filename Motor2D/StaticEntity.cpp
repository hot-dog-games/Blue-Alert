#include "j1App.h"
#include "Render.h"

#include "StaticEntity.h"



StaticEntity::StaticEntity()
{
	state = STATIC_IDLE;
}

StaticEntity::StaticEntity(pugi::xml_node entity_node, fPoint position) : Entity(entity_node, position)
{
	SetMaxLife(entity_node.attribute("life").as_uint());
}


StaticEntity::~StaticEntity()
{
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

bool StaticEntity::PostUpdate()
{
	App->render->Blit(sprite, position.x, position.y, &current_frame);

	return true;
}


void StaticEntity::Die()
{
	state = STATIC_DIE;
}