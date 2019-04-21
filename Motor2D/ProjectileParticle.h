#ifndef _PROJECTILE_PARTICLE_H_
#define _PROJECTILE_PARTICLE_H_

class SDL_Texture;
#include "Particle.h"


class ProjectileParticle :
	public Particle
{
public:
	ProjectileParticle(const pugi::xml_node &config, const fPoint &pos, const fPoint &destination, SDL_Texture* sprite);
	~ProjectileParticle();

	bool Update(float dt);
	bool PostUpdate();

protected:
	void Move(float dt);
	bool reached_dest = false;
	float speed;
	float angle;

	fPoint direction;
	fPoint destination;
};

#endif