#include "j1App.h"
#include "Render.h"
#include "Particles.h"
#include "Animation.h"
#include "Particle.h"



Particle::Particle(const pugi::xml_node &config, const fPoint &pos)
{
	position = pos;
	LoadAnimation(config.child("animation"));
}

void Particle::LoadAnimation(const pugi::xml_node &anim_node)
{
	pugi::xml_node frame;
	for (frame = anim_node.child("frame"); frame; frame = frame.next_sibling("frame"))
	{
		animation.PushBack({ frame.attribute("x").as_int(), frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });
	}

	animation.speed = anim_node.attribute("speed").as_float();
	animation.loop = false;
}

Particle::~Particle()
{

}

bool Particle::Update(float dt)
{
	if (animation.isDone())
		return false;

	current_frame = animation.GetCurrentFrame(dt);
	
	return true;
}

bool Particle::PostUpdate()
{
	App->render->Blit(sprite, position.x + (current_frame.w*0.5), position.y + (current_frame.h), &current_frame);

	return true;
}
