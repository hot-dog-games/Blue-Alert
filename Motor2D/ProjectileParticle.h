#ifndef _PROJECTILE_PARTICLE_H_
#define _PROJECTILE_PARTICLE_H_

class SDL_Texture;
#include "Particle.h"
#include "Particles.h"
#include "Entity.h"


class ProjectileParticle :
	public Particle
{
public:
	ProjectileParticle(const pugi::xml_node &config, const fPoint &pos, const fPoint &destination, SDL_Texture* sprite, bool rotate = true);
	~ProjectileParticle();

	bool Update(float dt);
	bool PostUpdate();
	void SetCollisionEffect(ParticleType effect, float radius = 0.0f, Faction own_faction = Faction::FACTION_NONE, float damage = 0.0f);

protected:
	void Move(float dt);
	bool reached_dest = false;

	float speed = 0.0f;
	float angle = 0.0f;
	float radius = 0.0f;
	float damage = 0.0f;

	fPoint direction = { 0.0f, 0.0f };
	fPoint destination = { 0.0f, 0.0f };
	ParticleType effect = ParticleType::NONE;
	Faction faction = Faction::FACTION_NONE;
	std::string fx = " ";
};

#endif