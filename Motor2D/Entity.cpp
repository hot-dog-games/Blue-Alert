#include "j1App.h"
#include "Textures.h"
#include "p2Log.h"

#include "Entity.h"


Entity::Entity()
{
}


Entity::~Entity()
{
	App->tex->UnLoad(sprite);
}

void Entity::DecreaseLife(float damage)
{
	if (current_life > 0)
	{
		current_life -= damage;
		if (current_life <= 0)
		{
			Die();
			current_life = 0;
		}
		LOG("current life is: %i", current_life);
	}


	
}

void Entity::SetMaxLife(uint new_life)
{
	max_life = new_life;
	current_life = max_life;
}

void Entity::LoadSprite(std::string sprite_path)
{
	sprite = App->tex->Load(sprite_path.c_str());
}