#include "j1App.h"
#include "Render.h"
#include "CardManager.h"
#include "DynamicEntity.h"



DynamicEntity::DynamicEntity()
{
}


DynamicEntity::~DynamicEntity()
{
}

DynamicEntity::DynamicEntity(pugi::xml_node config, fPoint position, Card* card, Faction faction): Entity(config, position, faction)
{
	entity_card = card;
}

bool DynamicEntity::PostUpdate()
{
	App->render->Blit(sprite, position.x, position.y, &current_frame);

	return true;
}

bool DynamicEntity::Update(float dt)
{
	switch (state)
	{
		case DYNAMIC_IDLE:
		{

		}
		break;
		case DYNAMIC_MOVING:
		{

		}
		break;
		case DYNAMIC_ATTACKING:
		{

		}
		break;
		case DYNAMIC_DYING:
		{

		}
		break;
	}

	current_frame = animations[state].GetCurrentFrame(dt);

	return true;
}