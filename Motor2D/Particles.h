#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "Module.h"

class Particle;
class SDL_Texture;

enum class ParticleType {
	NONE = -1,
	ATTACK_EXPLOSION,
	ATTACK_BASIC_SHOT,
	NUKE_BOMB,
	NUKE_EXPLOSION,
	ATTACK_MISSILE
};

class Particles : public Module
{
public:
	Particles();
	~Particles();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt);
	bool CleanUp();
	bool PostUpdate();
	bool Pause();
	bool Resume();
	bool Start();

	Particle* CreateParticle(const ParticleType &particle, const fPoint &pos, const fPoint &dest = {0.0F,0.0F});

private:
	SDL_Texture* particle_atlas = nullptr;
	std::string atlas_route;

	std::list<Particle*> particles;

	pugi::xml_document particle_file;
	pugi::xml_node particle_configs;

	bool paused = false;
};

#endif //__j1PARTICLES_H__
