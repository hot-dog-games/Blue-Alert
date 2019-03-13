#include "j1App.h"
#include "Render.h"

#include "StaticEntity.h"



StaticEntity::StaticEntity()
{
	state = STATIC_IDLE;
}


StaticEntity::~StaticEntity()
{
}

bool StaticEntity::Update(float dt)
{
	current_frame = animations[state].GetCurrentFrame(dt);

	if (state == STATIC_DIE && animations[state].isDone())
	{
		state = STATIC_DESTROYED;
	}

	return true;
}

bool StaticEntity::PostUpdate()
{
	App->render->Blit(sprite, position.x, position.y, &current_frame);

	return true;
}


void StaticEntity::LoadAnimations(pugi::xml_node anim_config)
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

void StaticEntity::Die()
{
	state = STATIC_DIE;
}