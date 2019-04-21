#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "Textures.h"
#include "Particle.h"
#include "ProjectileParticle.h"
#include "Particles.h"



Particles::Particles()
{
	name = "particles";
}


Particles::~Particles()
{
}

bool Particles::Awake(pugi::xml_node &config)
{
	std::string particles_route = config.child("xml").child_value();
	pugi::xml_parse_result result = particle_file.load_file(particles_route.c_str());

	if (result == NULL)
		LOG("Could not load entity xml. pugi error: %s", result.description());
	else
		particle_configs = particle_file.child("config");

	atlas_route = config.child("atlas").child_value();

	return true;
}

bool Particles::Start()
{
	particle_atlas = App->tex->Load(atlas_route.c_str());

	return true;
}

bool Particles::Update(float dt)
{
	if (!paused)
	{
		for (std::list<Particle*>::iterator particle = particles.begin(); particle != particles.end(); ++particle)
		{
			if (!(*particle)->Update(dt))
			{
				delete (*particle);
				particles.erase(particle);
			}		
		}
	}

	return true;
}

bool Particles::CleanUp()
{
	while (!particles.empty())
	{
		delete particles.front();
		particles.pop_front();
	}

	return true;
}

bool Particles::PostUpdate()
{
	for (std::list<Particle*>::iterator particle = particles.begin(); particle != particles.end(); ++particle)
	{
		(*particle)->PostUpdate();
	}

	return true;
}

Particle * Particles::CreateParticle(const ParticleType &particle_type, const fPoint &pos, const fPoint &dest)
{
	Particle* particle;
	pugi::xml_node particle_node;
	switch (particle_type)
	{
	case ParticleType::ATTACK_EXPLOSION:
		particle_node = particle_configs.find_child_by_attribute("type", std::to_string(0).c_str());
		particle = new Particle(particle_node, pos, particle_atlas);
		particle->type = particle_type;
		break;
	case ParticleType::ATTACK_BASIC_SHOT:
		particle_node = particle_configs.find_child_by_attribute("type", std::to_string(1).c_str());
		particle = new ProjectileParticle(particle_node, pos, dest, particle_atlas);
		particle->type = particle_type;
		break;
	default:
		break;
	}
	particles.push_back(particle);

	return particle;
}

bool Particles::Pause()
{
	paused = true;

	return true;
}
bool Particles::Resume()
{
	paused = false;

	return true;
}