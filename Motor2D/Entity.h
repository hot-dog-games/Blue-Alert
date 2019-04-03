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
	FACTION_NONE = -1,
	FACTION_RUSSIAN,
	FACTION_AMERICAN,

};

class Entity
{
public:
	Entity();
	Entity(pugi::xml_node entity_node, fPoint position, Faction faction);
	~Entity();

	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	void DecreaseLife(float damage);
	void SetMaxLife(uint new_life);
	void LoadAnimations(pugi::xml_node anim_config);
	void LoadSprite(pugi::xml_node node);
	virtual void Die() {};

public:
	fPoint position;
	Faction faction;

protected:
	std::vector<Animation> animations;
	SDL_Rect current_frame;
	SDL_Texture* sprite = nullptr;
	uint max_life;
	uint current_life;

	std::map<std::string, Stat*> stats;

};
#endif // _ENTITY_H_