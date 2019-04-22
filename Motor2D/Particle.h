#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "p2Point.h"
#include "Animation.h"

enum class ParticleType;

class Particle
{
public:
	Particle(const pugi::xml_node &config, const fPoint &pos, SDL_Texture* sprite);
	~Particle();

	virtual bool Update(float dt);
	virtual bool PostUpdate();

public:
	ParticleType type;

protected:
	virtual void LoadAnimation(const pugi::xml_node &config);
	Animation animation;
	SDL_Texture* sprite;
	SDL_Rect current_frame;

	fPoint position = { 0.0f,0.0f };
};

#endif
