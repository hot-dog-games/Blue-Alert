#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>
#include <map>

#include "p2Defs.h"
#include "p2Point.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;
class Stat;
enum EntityType;

enum PivotType {
	PVT_BOTTOM_CENTER,
	PVT_CENTER,
};
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
	virtual bool PostUpdate();
	virtual bool CleanUp() { return true; };
	virtual bool Start() { return true; };

	virtual void DecreaseLife(float damage, bool piercing = false);
	virtual float GetDamage() { return 0.0f; };
	void SetDebug(bool value);
	bool IsAlive();
	virtual bool IsArmored() { return false; };
	virtual int GetAttackType() { return -1; };
	void SetPivot(PivotType);
	fPoint GetCenterPosition();

protected:
	void LoadAnimations(pugi::xml_node anim_config);
	float CalculateDamage(float attack, float defense);
	virtual void Die() {};
	virtual void Draw();

public:
	fPoint position;
	Faction faction;
	EntityType type;
	SDL_Rect current_frame;
	int sorting_layer = 0;
	std::map<std::string, Stat*> stats;

protected:
	fPoint GetPosition();
	PivotType pivot = PVT_BOTTOM_CENTER;
	std::map<std::string, Animation> animations;
	Animation* current_animation = nullptr;
	SDL_Texture* sprite = nullptr;

	bool debug = false;


};
#endif // _ENTITY_H_