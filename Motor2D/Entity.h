#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>

#include "p2Defs.h"
#include "p2Point.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;
class Stat;

enum Faction {
	AMERICAN,
	RUSSIAN
};

class Entity
{
public:
	Entity();
	Entity(pugi::xml_node entity_node, fPoint position);
	~Entity();

	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	void DecreaseLife(float damage);
	void SetMaxLife(uint new_life);
	void LoadAnimations(pugi::xml_node anim_config);
	void LoadSprite(std::string sprite_path);
	virtual void Die() {};

public:
	fPoint position;

protected:
	std::vector<Animation> animations;
	SDL_Rect current_frame;
	SDL_Texture* sprite;
	uint max_life;
	uint current_life;
	Faction faction;

	std::map<std::string, Stat*> stats;

};
#endif // _ENTITY_H_