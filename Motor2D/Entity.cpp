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

Entity::Entity(pugi::xml_node entity_node, fPoint position)
{
	this->position = position;
	LoadAnimations(entity_node);
	LoadSprite(entity_node.child("sprite").child_value());
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

void Entity::LoadAnimations(pugi::xml_node anim_config)
{
	//load animations
	pugi::xml_node animation;
	for (animation = anim_config.child("animations").first_child(); animation; animation = animation.next_sibling())
	{
		Animation anim;
		pugi::xml_node frame;
		for (frame = animation.child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			anim.PushBack({ frame.attribute("x").as_int(), frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
		}
		anim.speed = animation.attribute("speed").as_float();
		anim.loop = animation.attribute("loop").as_bool(true);

		animations.push_back(anim);
	}
}