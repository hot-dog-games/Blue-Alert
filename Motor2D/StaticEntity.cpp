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
	fPoint render_position = { position.x - current_frame.w / 2, position.y - current_frame.h };
	App->render->Blit(sprite, render_position.x, render_position.y, &current_frame);

	LOG("state %i", state);
	LOG("RECT %i %i %i %i", current_frame.x, current_frame.y, current_frame.w, current_frame.h);

	return true;
}


void StaticEntity::Die()
{
	state = STATIC_DIE;
}

